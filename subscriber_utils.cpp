#include <iostream>
#include <cmath>
#include <iomanip>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

#include "subscriber_utils.hpp"

using namespace std;

void usage(char *filename) {
    cout << "Usage: " << filename << " ID_client IP_server PORT_server" << endl;
}

void print_topic(Topic *topic) {
    /*
     * Cast directly to struct in_addr in inet_ntoa call.
     *
     */
    cout << inet_ntoa({ .s_addr = topic->ip }) << ":" << topic->port << " ";
    cout << topic->topic << " ";

    /*
     * Switch based on the data type format to be displayed.
     *
     */
    switch (topic->data_type) {
        case (INT_BYTE):
            cout << INT << " ";
            break;

        case (SHORT_REAL_BYTE):
            cout << SHORT_REAL << " ";
            break;

        case (FLOAT_BYTE):
            cout << FLOAT << " ";
            break;

        case (STRING_BYTE):
            cout << STRING << " ";
            break;

        default:
            break;
    }

    /*
     * Cast the values appropriately.
     *
     */
    if (topic->data_type == INT_BYTE) {

        uint8_t *sign_byte = (uint8_t *) topic->value;
        uint32_t *value = (uint32_t *) (topic->value + sizeof(uint8_t));

        int result = htonl(*value);
        result = (*sign_byte == NEGATIVE) ? (-1) * (result) : result;

        cout << result;

    } else if (topic->data_type == SHORT_REAL_BYTE) {

        uint16_t *value = (uint16_t *) topic->value;
        cout << fixed << setprecision(2) << (float) ntohs(*value) / 100;

    } else if (topic->data_type == FLOAT_BYTE) {

        uint8_t *sign_byte = (uint8_t *) topic->value;
        uint32_t *value = (uint32_t *) (topic->value + sizeof(uint8_t));

        uint8_t *power = (uint8_t *) (topic->value + sizeof(uint8_t) + sizeof(uint32_t));

        double result = ntohl(*value) * pow(10, -(*power));
        result = (*sign_byte == NEGATIVE) ? (-1) * result : result;

        cout << fixed << setprecision(4) << (double) result;


    } else if (topic->data_type == STRING_BYTE) {

        cout << topic->value;

    }

    cout << endl;
}
