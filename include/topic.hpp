#ifndef TOPIC_HPP_
#define TOPIC_HPP_

#include <string.h>
#include <cstdint>

/*
 * Format of data types.
 *
 */
#define INT        "INT"
#define SHORT_REAL "SHORT_REAL"
#define FLOAT      "FLOAT"
#define STRING     "STRING"

/*
 * Identifiers for each data type.
 *
 */
#define INT_BYTE        0 
#define SHORT_REAL_BYTE 1
#define FLOAT_BYTE      2
#define STRING_BYTE     3

/*
 * Size of value field in topic for each data type.
 *
 */
#define INT_BYTE_VALUE_SIZE        5
#define SHORT_REAL_BYTE_VALUE_SIZE 2
#define FLOAT_BYTE_VALUE_SIZE      6

/*
 * Bytes for signedness.
 *
 */
#define POSITIVE 0
#define NEGATIVE 1

/*
 * Length of fields present in @Topic.
 *
 */
#define TOPIC_MAX_LEN     50
#define VALUE_MAX_LEN     1500

/*
 * Definition of structure that will be sent over the TCP conenction
 * to each subscriber.
 *
 */
struct Topic {
    uint32_t ip;
    uint16_t port;

    char     topic[TOPIC_MAX_LEN];
    uint8_t  data_type;
    size_t   value_sz;
    char     value[VALUE_MAX_LEN];

    /*
     * Compare all fields in topic. Use == for primitive data types and memcmp
     * for arrays.
     *
     */
    bool operator == (Topic& t) const {
        return ip == t.ip and port == t.port and !memcmp(topic, t.topic, TOPIC_MAX_LEN)
            and data_type == t.data_type and value_sz == t.value_sz
            and !memcmp(value, t.value, VALUE_MAX_LEN);
    }


}__attribute__((packed));

#endif
