#ifndef TYPES_H

#define TYPES_H

typedef enum
{
    e_success,
    e_failure

}status;

typedef enum
{
    e_view,
    e_edit,
    e_help,
    e_unsupported

}operation_type;

typedef enum
{
    e_Title,
    e_Artist,
    e_Album,
    e_Year,
    e_Comment,
    e_Genre,
    e_Invalid_field

}field;



#endif
