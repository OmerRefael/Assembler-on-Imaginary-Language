#ifndef ADDRESS_METHOD_H
#define ADDRESS_METHOD_H

typedef enum{
    no_valid_address_method = 0,
    immediate_adress = 1,
    direct_adress = 2,
    indirect_register_adress = 4,
    direct_register_adress = 8

}address_method;

#endif