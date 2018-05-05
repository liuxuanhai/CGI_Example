/******************************************    *******************************

      > File Name: protocol.h

      > Author: Ywl

      > Descripsion:

      > Created Time:     Sat 05 May 2018 01:02:46 AM PDT

      > Modify Time: 

 *********************************    ***************************************/

#ifndef _PROTOCOL_H
#define _PROTOCOL_H

typedef struct{
    char len;
    int id;
    char pdu[0];
}c_class_parameter;

typedef struct{
    char rw;
    char ack;
    char pdu[0];
}c_class_packet;

typedef const struct{
    char *str;
    int id;
}c_class_id;

typedef const struct _c_class_parameter_map{
    c_class_id id;
    char data_len;
    char data_type;
    char rw_permission;
    const char initial_value;
    void *data_addr;
    c_class_packet *(*getset)(c_class_parameter parameter*, c_common_packet *packet, const struct _c_class_parameter_map *map, unsigned int rw);
    c_class_packet *(*check)(c_class_parameter parameter*, c_common_packet *packet, const struct _c_class_parameter_map *map, unsigned int rw);
    char (*valid)(void);
    const char *section;
    char reverse[8];
}c_class_parameter_map;

typedef const struct _c_class_module{
    char get;
    char set;
    char addr;
    c_class_parameter *definite_module;
    c_common_packet *(*handler)(c_common_packet *packet, const struct _c_class_module *module);
    const char *xmlNode;
    void (*poll)(const struct _c_class_module *module); 
}c_class_module;

#endif
