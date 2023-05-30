#include "origin_defi_plugin.h"

// Set UI for the "Send" screen.
/*static void set_send_ui(ethQueryContractUI_t *msg, origin_defi_parameters_t *context) {
    uint8_t decimals = 0;
    char *ticker;

    switch (context->selectorIndex) {
        case ZAPPER_DEPOSIT_ETH:
        case ZAPPER_DEPOSIT_SFRXETH:
        case VAULT_MINT:
            strlcpy(msg->title, "Deposit", msg->titleLength);
            decimals = WEI_TO_ETHER;
            ticker = "ETH";
            break;
        case VAULT_REDEEM:
            strlcpy(msg->title, "Send", msg->titleLength);
            decimals = OETH_DECIMALS;
            ticker = OETH_TICKER;
            break;
        case CURVE_EXCHANGE:
            strlcpy(msg->title, "Send", msg->titleLength);
            if (ADDRESS_IS_NETWORK_TOKEN(context->contract_address_sent)) {
                strlcpy(context->ticker_sent, msg->network_ticker, sizeof(context->ticker_sent));
            }
            decimals = WEI_TO_ETHER;
            ticker = "ETH";
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    amountToString(context->amount_sent,
                   INT256_LENGTH,
                   decimals,
                   ticker,
                   msg->msg,
                   msg->msgLength);
}

static void set_receive_ui(ethQueryContractUI_t *msg, origin_defi_parameters_t *context) {
    uint8_t decimals = 0;
    char *ticker;
    strlcpy(msg->title, "Receive Min", msg->titleLength);

    switch (context->selectorIndex) {
        case ZAPPER_DEPOSIT_ETH:
        case ZAPPER_DEPOSIT_SFRXETH:
        case VAULT_MINT:
            decimals = OETH_DECIMALS;
            ticker = OETH_TICKER;
            break;
        case VAULT_REDEEM:
            decimals = WEI_TO_ETHER;
            ticker = "UNITS";
            break;
        case CURVE_EXCHANGE:
            if (ADDRESS_IS_NETWORK_TOKEN(context->contract_address_received)) {
                //strlcpy(context->ticker_sent, msg->network_ticker, sizeof(context->ticker_sent));
                decimals = WEI_TO_ETHER;
                ticker = "ETH";
            } else {
                decimals = OETH_DECIMALS;
                ticker = OETH_TICKER;
            }
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    amountToString(context->amount_sent,
                   INT256_LENGTH,
                   decimals,
                   ticker,
                   msg->msg,
                   msg->msgLength);
}*/

/*static void set_recipient_ui(ethQueryContractUI_t *msg, origin_defi_parameters_t *context) {
    strlcpy(msg->title, "Recipient", msg->titleLength);
    msg->msg[0] = '0';
    msg->msg[1] = 'x';
    uint64_t chainid = 0;
    getEthAddressStringFromBinary(msg->address,
                                  msg->msg + 2,
                                  msg->pluginSharedR0->txContent,
                                  chainid);
}*/

static void set_send_ui(ethQueryContractUI_t *msg, origin_defi_parameters_t *context) {

    // set network ticker (ETH, BNB, etc) if needed
    if (ADDRESS_IS_NETWORK_TOKEN(context->contract_address_sent)) {
        strlcpy(context->ticker_sent, msg->network_ticker, sizeof(context->ticker_sent));
    }

    if (ADDRESS_IS_OETH(context->contract_address_sent)) {
        strlcpy(context->ticker_sent, OETH_TICKER, sizeof(context->ticker_sent));
    }

    switch (context->selectorIndex) {
        case ZAPPER_DEPOSIT_ETH:
            strlcpy(msg->title, "Deposit", msg->titleLength);
            break;
        case ZAPPER_DEPOSIT_SFRXETH:
            strlcpy(msg->title, "Deposit", msg->titleLength);
            strlcpy(context->ticker_sent, SFRXETH_TICKER, sizeof(context->ticker_sent));
            break;
        case VAULT_MINT:
            strlcpy(msg->title, "Deposit", msg->titleLength);
            break;
        case VAULT_REDEEM:
            strlcpy(msg->title, "Redeem", msg->titleLength);
            if (memcmp(OETH_VAULT_ADDRESS, msg->pluginSharedRO->txContent->destination, ADDRESS_LENGTH) == 0) {
                strlcpy(context->ticker_sent, OETH_TICKER, sizeof(context->ticker_sent));
            } else {
                strlcpy(context->ticker_sent, OUSD_TICKER, sizeof(context->ticker_sent));
            }
            break;
        case CURVE_EXCHANGE:
            strlcpy(msg->title, "Send", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    // Convert to string.
    amountToString(context->amount_sent,
                   INT256_LENGTH,
                   context->decimals_sent,
                   context->ticker_sent,
                   msg->msg,
                   msg->msgLength);
    PRINTF("AMOUNT SENT: %s\n", msg->msg);
}

// Set UI for "Receive" screen.
static void set_receive_ui(ethQueryContractUI_t *msg, origin_defi_parameters_t *context) {
    
     // set network ticker (ETH, BNB, etc) if needed
    if (ADDRESS_IS_NETWORK_TOKEN(context->contract_address_received)) {
        strlcpy(context->ticker_received, msg->network_ticker, sizeof(context->ticker_received));
    }

    if (ADDRESS_IS_OETH(context->contract_address_received)) {
        strlcpy(context->ticker_received, OETH_TICKER, sizeof(context->ticker_received));
    }

    switch (context->selectorIndex) {
        case ZAPPER_DEPOSIT_ETH:
        case ZAPPER_DEPOSIT_SFRXETH:
        case VAULT_MINT:
            strlcpy(msg->title, "Receive Min", msg->titleLength);
            strlcpy(context->ticker_received, OETH_TICKER, sizeof(context->ticker_received));
            break;
        case VAULT_REDEEM:
            strlcpy(msg->title, "Receive Min", msg->titleLength);
            strlcpy(context->ticker_received, ETH_UNITS_TICKER, sizeof(context->ticker_received));
            break;
        case CURVE_EXCHANGE:
            strlcpy(msg->title, "Receive Min", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    // Convert to string.
    amountToString(context->min_amount_received,
                   INT256_LENGTH,
                   context->decimals_received,
                   context->ticker_received,
                   msg->msg,
                   msg->msgLength);
    PRINTF("AMOUNT RECEIVED: %s\n", msg->msg);
}

// Set UI for "Warning" screen.
static void set_warning_ui(ethQueryContractUI_t *msg,
                           const origin_defi_parameters_t *context __attribute__((unused))) {
    strlcpy(msg->title, "WARNING", msg->titleLength);
    strlcpy(msg->msg, "Unknown token", msg->msgLength);
}

// Helper function that returns the enum corresponding to the screen that should be displayed.
static screens_t get_screen(const ethQueryContractUI_t *msg, const origin_defi_parameters_t *context) {
    uint8_t index = msg->screenIndex;

    bool token_sent_found = context->tokens_found & TOKEN_SENT_FOUND;
    bool token_received_found = context->tokens_found & TOKEN_RECEIVED_FOUND;

    bool both_tokens_found = token_received_found && token_sent_found;
    bool both_tokens_not_found = !token_received_found && !token_sent_found;

    switch (index) {
        case 0:
            if (both_tokens_found) {
                return SEND_SCREEN;
            } else if (both_tokens_not_found) {
                return WARN_SCREEN;
            } else if (token_sent_found) {
                return SEND_SCREEN;
            } else if (token_received_found) {
                return WARN_SCREEN;
            }
            break;
        case 1:
            if (both_tokens_found) {
                return RECEIVE_SCREEN;
            } else if (both_tokens_not_found) {
                return SEND_SCREEN;
            } else if (token_sent_found) {
                return WARN_SCREEN;
            } else if (token_received_found) {
                return SEND_SCREEN;
            }
            break;
        default:
            return ERROR;
            break;
    }
    return ERROR;
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    origin_defi_parameters_t *context = (origin_defi_parameters_t *) msg->pluginContext;
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);
    msg->result = ETH_PLUGIN_RESULT_OK;
    screens_t screen = get_screen(msg, context);
    switch (screen) {
        case SEND_SCREEN:
            set_send_ui(msg, context);
            break;
        case RECEIVE_SCREEN:
            set_receive_ui(msg, context);
            break;
        case WARN_SCREEN:
            set_warning_ui(msg, context);
            break;
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}