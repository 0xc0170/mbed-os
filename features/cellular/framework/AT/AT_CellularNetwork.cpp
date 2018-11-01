/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include "AT_CellularNetwork.h"
#include "CellularUtil.h"
#include "CellularLog.h"
#include "CellularCommon.h"

using namespace std;
using namespace mbed_cellular_util;
using namespace mbed;

struct at_reg_t {
    const CellularNetwork::RegistrationType type;
    const char *const cmd;
    const char *const urc_prefix;
};

static const at_reg_t at_reg[] = {
    { CellularNetwork::C_EREG, "AT+CEREG", "+CEREG:"},
    { CellularNetwork::C_GREG, "AT+CGREG", "+CGREG:"},
    { CellularNetwork::C_REG,  "AT+CREG", "+CREG:"}
};

AT_CellularNetwork::AT_CellularNetwork(ATHandler &atHandler) : AT_CellularBase(atHandler),
    _connection_status_cb(NULL), _op_act(RAT_UNKNOWN), _connect_status(NSAPI_STATUS_DISCONNECTED)
{

    _urc_funcs[C_EREG] = callback(this, &AT_CellularNetwork::urc_cereg);
    _urc_funcs[C_GREG] = callback(this, &AT_CellularNetwork::urc_cgreg);
    _urc_funcs[C_REG] = callback(this, &AT_CellularNetwork::urc_creg);

    for (int type = 0; type < CellularNetwork::C_MAX; type++) {
        if (has_registration((RegistrationType)type) != RegistrationModeDisable) {
            _at.set_urc_handler(at_reg[type].urc_prefix, _urc_funcs[type]);
        }
    }

    _at.set_urc_handler("NO CARRIER", callback(this, &AT_CellularNetwork::urc_no_carrier));
    // additional urc to get better disconnect info for application. Not critical.
    _at.set_urc_handler("+CGEV:", callback(this, &AT_CellularNetwork::urc_cgev));
    _at.lock();
    _at.cmd_start("AT+CGEREP=1");// discard unsolicited result codes when MT TE link is reserved (e.g. in on line data mode); otherwise forward them directly to the TE
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    _at.unlock();
}

AT_CellularNetwork::~AT_CellularNetwork()
{
    _at.lock();
    _at.cmd_start("AT+CGEREP=0");// buffer unsolicited result codes in the MT; if MT result code buffer is full, the oldest ones can be discarded. No codes are forwarded to the TE
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    _at.unlock();

    for (int type = 0; type < CellularNetwork::C_MAX; type++) {
        if (has_registration((RegistrationType)type) != RegistrationModeDisable) {
            _at.remove_urc_handler(at_reg[type].urc_prefix);
        }
    }

    _at.remove_urc_handler("NO CARRIER");
    _at.remove_urc_handler("+CGEV:");
}

void AT_CellularNetwork::urc_no_carrier()
{
    tr_error("Data call failed: no carrier");
    call_network_cb(NSAPI_STATUS_DISCONNECTED);
}

void AT_CellularNetwork::urc_cgev()
{
    char buf[13];
    if (_at.read_string(buf, 13) < 8) { // smallest string length we wan't to compare is 8
        return;
    }
    tr_debug("urc_cgev: %s", buf);

    bool call_cb = false;
    // NOTE! If in future there will be 2 or more active contexts we might wan't to read context id also but not for now.

    if (memcmp(buf, "NW DETACH", 9) == 0) { // The network has forced a PS detach
        call_cb = true;
    } else if (memcmp(buf, "ME DETACH", 9) == 0) {// The mobile termination has forced a PS detach.
        call_cb = true;
    } else if (memcmp(buf, "NW DEACT", 8) == 0) {// The network has forced a context deactivation
        call_cb = true;
    } else if (memcmp(buf, "ME DEACT", 8) == 0) {// The mobile termination has forced a context deactivation
        call_cb = true;
    } else if (memcmp(buf, "NW PDN DEACT", 12) == 0) {// The network has deactivated a context
        call_cb = true;
    } else if (memcmp(buf, "ME PDN DEACT", 12) == 0) {// The mobile termination has deactivated a context.
        call_cb = true;
    }

    if (call_cb) {
        call_network_cb(NSAPI_STATUS_DISCONNECTED);
    }
}

void AT_CellularNetwork::read_reg_params_and_compare(RegistrationType type)
{
    registration_params_t reg_params;
    read_reg_params(reg_params);

#if MBED_CONF_MBED_TRACE_ENABLE
    switch (reg_params._status) {
        case NotRegistered:
            tr_warn("not registered");
            break;
        case RegistrationDenied:
            tr_warn("registration denied");
            break;
        case Unknown:
            tr_warn("registration status unknown");
            break;
        default:
            break;
    }
#endif

    if (_at.get_last_error() == NSAPI_ERROR_OK && _connection_status_cb) {
        tr_debug("type: %d, status: %d, lac: %d, cellID: %d, act: %d", type, reg_params._status, reg_params._lac, reg_params._cell_id, reg_params._act);
        _reg_params._type = type;
        cell_callback_data_t data;
        data.error = NSAPI_ERROR_OK;
        if (reg_params._act != _reg_params._act) {
            _reg_params._act = reg_params._act;
            data.status_data = reg_params._act;
            _connection_status_cb((nsapi_event_t)CellularRadioAccessTechnologyChanged, (intptr_t)&data);
        }
        if (reg_params._status != _reg_params._status) {
            _reg_params._status = reg_params._status;
            data.status_data = reg_params._status;
            _connection_status_cb((nsapi_event_t)CellularRegistrationStatusChanged, (intptr_t)&data);
        }
        if (reg_params._cell_id != -1 && reg_params._cell_id != _reg_params._cell_id) {
            _reg_params._cell_id = reg_params._cell_id;
            data.status_data = reg_params._cell_id;
            _connection_status_cb((nsapi_event_t)CellularCellIDChanged, (intptr_t)&data);
        }
    }
}

void AT_CellularNetwork::urc_creg()
{
    tr_debug("urc_creg");
    read_reg_params_and_compare(C_REG);
}

void AT_CellularNetwork::urc_cereg()
{
    tr_debug("urc_cereg");
    read_reg_params_and_compare(C_EREG);
}

void AT_CellularNetwork::urc_cgreg()
{
    tr_debug("urc_cgreg");
    read_reg_params_and_compare(C_GREG);
}

void AT_CellularNetwork::call_network_cb(nsapi_connection_status_t status)
{
    if (_connect_status != status) {
        _connect_status = status;
        if (_connection_status_cb) {
            _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, _connect_status);
        }
    }
}

void AT_CellularNetwork::attach(Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _connection_status_cb = status_cb;
}

nsapi_connection_status_t AT_CellularNetwork::get_connection_status() const
{
    return _connect_status;
}

nsapi_error_t AT_CellularNetwork::set_registration_urc(RegistrationType type, bool urc_on)
{
    int index = (int)type;
    MBED_ASSERT(index >= 0 && index < C_MAX);

    RegistrationMode mode = has_registration(type);
    if (mode == RegistrationModeDisable) {
        return NSAPI_ERROR_UNSUPPORTED;
    } else {
        _at.lock();
        if (urc_on) {
            _at.cmd_start(at_reg[index].cmd);
            const uint8_t ch_eq = '=';
            _at.write_bytes(&ch_eq, 1);
            _at.write_int((int)mode);
        } else {
            _at.cmd_start(at_reg[index].cmd);
            _at.write_string("=0", false);
        }

        _at.cmd_stop_read_resp();
        return _at.unlock_return_error();
    }
}

nsapi_error_t AT_CellularNetwork::get_network_registering_mode(NWRegisteringMode &mode)
{
    _at.lock();
    _at.cmd_start("AT+COPS?");
    _at.cmd_stop();
    _at.resp_start("+COPS:");
    mode = (NWRegisteringMode)_at.read_int();
    _at.resp_stop();

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::set_registration(const char *plmn)
{
    _at.lock();

    if (!plmn) {
        tr_debug("Automatic network registration");
        _at.cmd_start("AT+COPS?");
        _at.cmd_stop();
        _at.resp_start("+COPS:");
        int mode = _at.read_int();
        _at.resp_stop();
        if (mode != 0) {
            _at.clear_error();
            _at.cmd_start("AT+COPS=0");
            _at.cmd_stop_read_resp();
        }
    } else {
        tr_debug("Manual network registration to %s", plmn);
        _at.cmd_start("AT+COPS=4,2,");
        _at.write_string(plmn);
        _at.cmd_stop_read_resp();
    }

    return _at.unlock_return_error();
}

void AT_CellularNetwork::read_reg_params(registration_params_t &reg_params)
{
    const int MAX_STRING_LENGTH = 9;
    char string_param[MAX_STRING_LENGTH] = {0};

    int int_param = _at.read_int();
    reg_params._status = (RegistrationStatus)int_param;

    int len = _at.read_string(string_param, TWO_BYTES_HEX + 1);
    if (len > 0) {
        reg_params._lac = hex_str_to_int(string_param, TWO_BYTES_HEX);
        tr_debug("lac %s %d", string_param, reg_params._lac);
    } else {
        reg_params._lac = -1;
    }

    len = _at.read_string(string_param, FOUR_BYTES_HEX + 1);
    if (len > 0) {
        reg_params._cell_id = hex_str_to_int(string_param, FOUR_BYTES_HEX);
        tr_debug("cell_id %s %d", string_param, reg_params._cell_id);
    } else {
        reg_params._cell_id = -1;
    }

    int_param = _at.read_int();
    reg_params._act = (int_param == -1) ? RAT_UNKNOWN : (RadioAccessTechnology)int_param;

    // Skip [<cause_type>],[<reject_cause>]
    _at.skip_param(2);

    len = _at.read_string(string_param, ONE_BYTE_BINARY + 1);
    reg_params._active_time = calculate_active_time(string_param, len);
    if (reg_params._active_time != -1) {
        tr_debug("active_time %s %d", string_param, reg_params._active_time);
    }

    len = _at.read_string(string_param, ONE_BYTE_BINARY + 1);
    reg_params._periodic_tau = calculate_periodic_tau(string_param, len);
    if (reg_params._periodic_tau == -1) {
        tr_debug("periodic_tau %s %d", string_param, reg_params._periodic_tau);
    }
}

AT_CellularNetwork::RegistrationMode AT_CellularNetwork::has_registration(RegistrationType reg_type)
{
    (void)reg_type;
    return RegistrationModeLAC;
}

nsapi_error_t AT_CellularNetwork::set_attach()
{
    _at.lock();

    _at.cmd_start("AT+CGATT?");
    _at.cmd_stop();
    _at.resp_start("+CGATT:");
    int attached_state = _at.read_int();
    _at.resp_stop();
    if (attached_state != 1) {
        tr_debug("Network attach");
        _at.cmd_start("AT+CGATT=1");
        _at.cmd_stop_read_resp();
    }

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_attach(AttachStatus &status)
{
    _at.lock();

    _at.cmd_start("AT+CGATT?");
    _at.cmd_stop();

    _at.resp_start("+CGATT:");
    if (_at.info_resp()) {
        int attach_status = _at.read_int();
        status = (attach_status == 1) ? Attached : Detached;
    }
    _at.resp_stop();

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::detach()
{
    _at.lock();

    tr_debug("Network detach");
    _at.cmd_start("AT+CGATT=0");
    _at.cmd_stop_read_resp();

    call_network_cb(NSAPI_STATUS_DISCONNECTED);

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::set_access_technology_impl(RadioAccessTechnology opsAct)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t AT_CellularNetwork::set_access_technology(RadioAccessTechnology opAct)
{
    if (opAct == RAT_UNKNOWN) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    _op_act = opAct;

    return set_access_technology_impl(opAct);
}

nsapi_error_t AT_CellularNetwork::scan_plmn(operList_t &operators, int &opsCount)
{
    int idx = 0;

    _at.lock();

    _at.cmd_start("AT+COPS=?");
    _at.cmd_stop();

    _at.resp_start("+COPS:");

    int ret, error_code = -1;
    operator_t *op = NULL;

    while (_at.info_elem('(')) {

        op = operators.add_new();
        op->op_status = (operator_t::Status)_at.read_int();
        _at.read_string(op->op_long, sizeof(op->op_long));
        _at.read_string(op->op_short, sizeof(op->op_short));
        _at.read_string(op->op_num, sizeof(op->op_num));

        // Optional - try read an int
        ret = _at.read_int();
        op->op_rat = (ret == error_code) ? RAT_UNKNOWN : (RadioAccessTechnology)ret;

        if ((_op_act == RAT_UNKNOWN) ||
                ((op->op_rat != RAT_UNKNOWN) && (op->op_rat == _op_act))) {
            idx++;
        } else {
            operators.delete_last();
        }
    }

    _at.resp_stop();

    opsCount = idx;
    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::set_ciot_optimization_config(Supported_UE_Opt supported_opt,
                                                               Preferred_UE_Opt preferred_opt)
{

    _at.lock();

    _at.cmd_start("AT+CCIOTOPT=");
    _at.write_int(0); // disable urc
    _at.write_int(supported_opt);
    _at.write_int(preferred_opt);
    _at.cmd_stop_read_resp();

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_ciot_optimization_config(Supported_UE_Opt &supported_opt,
                                                               Preferred_UE_Opt &preferred_opt)
{
    _at.lock();

    _at.cmd_start("AT+CCIOTOPT?");
    _at.cmd_stop();

    _at.resp_start("+CCIOTOPT:");
    _at.read_int();
    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        supported_opt = (Supported_UE_Opt)_at.read_int();
        preferred_opt = (Preferred_UE_Opt)_at.read_int();
    }

    _at.resp_stop();

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_extended_signal_quality(int &rxlev, int &ber, int &rscp, int &ecno, int &rsrq, int &rsrp)
{
    _at.lock();

    _at.cmd_start("AT+CESQ");
    _at.cmd_stop();

    _at.resp_start("+CESQ:");
    rxlev = _at.read_int();
    ber = _at.read_int();
    rscp = _at.read_int();
    ecno = _at.read_int();
    rsrq = _at.read_int();
    rsrp = _at.read_int();
    _at.resp_stop();
    if (rxlev < 0 || ber < 0 || rscp < 0 || ecno < 0 || rsrq < 0 || rsrp < 0) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_signal_quality(int &rssi, int &ber)
{
    _at.lock();

    _at.cmd_start("AT+CSQ");
    _at.cmd_stop();

    _at.resp_start("+CSQ:");
    rssi = _at.read_int();
    ber = _at.read_int();
    _at.resp_stop();
    if (rssi < 0 || ber < 0) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    return _at.unlock_return_error();
}

/** Get the last 3GPP error code
 *  @return see 3GPP TS 27.007 error codes
 */
int AT_CellularNetwork::get_3gpp_error()
{
    return _at.get_3gpp_error();
}

nsapi_error_t AT_CellularNetwork::get_operator_params(int &format, operator_t &operator_params)
{
    _at.lock();

    _at.cmd_start("AT+COPS?");
    _at.cmd_stop();

    _at.resp_start("+COPS:");
    _at.read_int(); //ignore mode
    format = _at.read_int();

    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        switch (format) {
            case 0:
                _at.read_string(operator_params.op_long, sizeof(operator_params.op_long));
                break;
            case 1:
                _at.read_string(operator_params.op_short, sizeof(operator_params.op_short));
                break;
            default:
                _at.read_string(operator_params.op_num, sizeof(operator_params.op_num));
                break;
        }
        operator_params.op_rat = (RadioAccessTechnology)_at.read_int();
    }

    _at.resp_stop();

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_operator_names(operator_names_list &op_names)
{
    _at.lock();

    _at.cmd_start("AT+COPN");
    _at.cmd_stop();

    _at.resp_start("+COPN:");
    operator_names_t *names = NULL;
    while (_at.info_resp()) {
        names = op_names.add_new();
        _at.read_string(names->numeric, sizeof(names->numeric));
        _at.read_string(names->alpha, sizeof(names->alpha));
    }

    _at.resp_stop();
    return _at.unlock_return_error();
}

bool AT_CellularNetwork::is_active_context()
{
    _at.lock();

    bool active_found = false;
    // read active contexts
    _at.cmd_start("AT+CGACT?");
    _at.cmd_stop();
    _at.resp_start("+CGACT:");
    while (_at.info_resp()) {
        (void)_at.read_int(); // discard context id
        if (_at.read_int() == 1) { // check state
            tr_debug("Found active context");
            active_found = true;
            break;
        }
    }
    _at.resp_stop();
    _at.unlock();

    return active_found;
}

nsapi_error_t AT_CellularNetwork::get_registration_params(registration_params_t &reg_params)
{
    reg_params = _reg_params;
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_registration_params(RegistrationType type, registration_params_t &reg_params)
{
    int i = (int)type;
    MBED_ASSERT(i >= 0 && i < C_MAX);

    if (!has_registration(at_reg[i].type)) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    _at.lock();

    const char *rsp[] = { "+CEREG:", "+CGREG:", "+CREG:"};
    _at.cmd_start(at_reg[i].cmd);
    _at.write_string("?", false);
    _at.cmd_stop();
    _at.resp_start(rsp[i]);

    (void)_at.read_int(); // ignore urc mode subparam
    _reg_params._type = type;
    read_reg_params(reg_params);
    _at.resp_stop();

    _reg_params = reg_params;

    return _at.unlock_return_error();
}

int AT_CellularNetwork::calculate_active_time(const char *active_time_string, int active_time_length)
{
    if (active_time_length != ONE_BYTE_BINARY) {
        return -1;
    }

    uint32_t ie_unit = binary_str_to_uint(active_time_string, TIMER_UNIT_LENGTH);
    uint32_t ie_value = binary_str_to_uint(active_time_string + TIMER_UNIT_LENGTH, active_time_length - TIMER_UNIT_LENGTH);

    switch (ie_unit) {
        case 0: // multiples of 2 seconds
            return 2 * ie_value;
        case 1: // multiples of 1 minute
            return 60 * ie_value;
        case 2: // multiples of decihours
            return 6 * 60 * ie_value;
        case 7: // timer is deactivated
            return 0;
        default: // other values shall be interpreted as multiples of 1 minute
            return 60 * ie_value;
    }
}

int AT_CellularNetwork::calculate_periodic_tau(const char *periodic_tau_string, int periodic_tau_length)
{
    if (periodic_tau_length != ONE_BYTE_BINARY) {
        return -1;
    }

    uint32_t ie_unit = binary_str_to_uint(periodic_tau_string, TIMER_UNIT_LENGTH);
    uint32_t ie_value = binary_str_to_uint(periodic_tau_string + TIMER_UNIT_LENGTH, periodic_tau_length - TIMER_UNIT_LENGTH);

    switch (ie_unit) {
        case 0: // multiples of 10 minutes
            return 60 * 10 * ie_value;
        case 1: // multiples of 1 hour
            return 60 * 60 * ie_value;
        case 2: // multiples of 10 hours
            return 10 * 60 * 60 * ie_value;
        case 3: // multiples of 2 seconds
            return 2 * ie_value;
        case 4: // multiples of 30 seconds
            return 30 * ie_value;
        case 5: // multiples of 1 minute
            return 60 * ie_value;
        case 6: // multiples of 320 hours
            return 320 * 60 * 60 * ie_value;
        default: // timer is deactivated
            return 0;
    }
}
