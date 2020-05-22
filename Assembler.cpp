#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <cmath>
#include "files.h"
#include "general.h"

#include "enum.h"

struct reg_name
    {
    char names[reg_name_size] = "";
    };

struct label_t
    {
    char name[15]= "POTION";
    int addres = -1;
    };

struct label_array_t
    {
    label_t* data = nullptr;
    int current_size = 0;
    int max_size = label_array_default_size;
    };

int label_joining( label_array_t* ptr, const char name[] ) //creation or joining by given label name
    {
    assert( ptr );
    for (int i = 0; i < ptr->current_size; i++ )
        {
        if( strcmp( name , ( *( ptr->data+i ) ).name ) == 0 )
            {
            return 0;
            }
        }
    strcpy( (char*) (ptr->data + ptr->current_size )  , name );
    ptr->current_size++;
    }

int label_find( label_array_t* ptr, const char name[] )
    {
    assert( ptr );
    for (int i = 0; i < ptr->current_size; i++ )
        {
        if( strcmp( name , ( *( ptr->data+i ) ).name ) == 0 )
            {
            return i;
            }
        }
    return -1;
    }

void label_array_init( label_array_t* ptr, int  size = label_array_default_size )
    {
    assert(ptr);
    ptr->data = (label_t*) calloc(size, sizeof(label_t) );
    ptr->max_size = size;
    ptr->current_size = 0;

    assert(ptr);
    }

void reg_init(reg_name* ptr)
    {
    #define DEF_REG(name,num) strcpy(ptr[num].names,  #name);
    #include "registers.h"
    #undef DEF_REG
    }


bool check_reg_argue(char reg_name[15])
    {
    #define DEF_REG(name, ind) && strcmp(#name,reg_name)
    bool check_mistake = 1
    #include "registers.h";
    #undef DEF_REG
    ;
    return check_mistake;
    }

int reg_num(const char reg_name[15])
    {
    #define DEF_REG(name, ind) \
    if ( !strcmp(#name,reg_name) )\
        {\
         return REG__##name;\
        }

    #include "registers.h"

    #undef DEF_REG
    return -1;
    }

void shifting( char* ptr, int* shift, size_t add = 0)
    {
    *shift+=add;
    while( isspace(ptr[*shift]) != 0)
        {
        (*shift)++;
        }
    }

int labelwork( char* ptr, int* shift, label_array_t* label_array )
    {
    char curr_arg[15] = "";
    sscanf(ptr + *shift, " %s ", curr_arg);
    int current_arg = label_find( label_array, curr_arg );
    if (current_arg == -1)
        {
        return -1;
        }
    shifting( ptr, shift, strlen(curr_arg) );
    return label_array->data[current_arg].addres;
    }

int regwork( char* ptr, int* shift )
    {
    char curr_arg[15] = "";
    sscanf(ptr + *shift, " %[a-z]s ", curr_arg);
    int current_arg = reg_num(curr_arg);
    if (current_arg != -1)
        {
        shifting( ptr, shift, strlen(curr_arg) );
        }
    return  current_arg;
    }

void argwork( char* ptr, int* shift, int* arg, label_array_t* labelarr, bool label, bool reg, bool lable_list_comp )
    {
    int current_arg = 0;
    if ( reg )
        {
        *arg = regwork( ptr, shift );
        if ( *arg != -1 )
            {
            return;
            }
        }
    double double_arg = NAN ;
    if (0 != sscanf(ptr + *shift, "%lg%n", &double_arg, &current_arg ) )
        {
        *arg = round( (double_arg*accuracy_mul) );
        shifting( ptr, shift, current_arg );
        return ;
        }
    if ( label )
        {
        *arg = labelwork( ptr, shift, labelarr);
        if ( lable_list_comp || (*arg != -1) )
            {
            return;
            }
        }
    printf("invalid argument\n");
    exit(220);
    }

void pushram_extract( char* ptr, int* arg1, int* arg2 )
    {
    char current[30] = "";
    sscanf( ptr,"%[^]]",current);
    int shift = 0;
    shifting( current, &shift );
    char curr_arg[15] = "";
    sscanf(current+shift, "%[a-z]" , curr_arg);
    *arg1 = reg_num(curr_arg);
    sscanf(current, "%[a-z+ \t]%n" , curr_arg, &shift);
    sscanf(current+shift, "%d" , arg2);
    *arg2 *=accuracy_mul;
    }

void pushargwork( char* ptr, int* shift, int* num, FILE* machine )
    {
    int current_arg = 0;
        current_arg = regwork( ptr, shift );
        if ( current_arg != -1 )
            {
            *num = 10;
            fwrite( num, sizeof(char), 1, machine);
            fwrite( &current_arg, sizeof(int), 1, machine);
            return;
            }
    double double_arg = NAN ;
    if (0 != sscanf(ptr + *shift, "%lg%n", &double_arg, &current_arg ) )
        {
        current_arg = round( (double_arg*accuracy_mul) );
        *num = 1;
        fwrite( num, sizeof(char), 1, machine);
        fwrite( &current_arg, sizeof(int), 1, machine);
        return ;
        }
        if ( strstr(ptr + *shift,"[") != nullptr )
            {
            *num = 11;
            if ( strstr( ptr + *shift,"RAM") != nullptr )
                {
                *num = 21;
                }
            int count = -1;
            fwrite( num, sizeof(char), 1, machine);
            pushram_extract( strstr( ptr + *shift,"[")+1, &current_arg, &count );
            fwrite( &current_arg, sizeof(int), 1,machine);
            fwrite( &count, sizeof(int), 1,machine);
            return;
            }
    printf("invalid push argument\n");
    exit(219);
    }
    
void label_init( char* ptr, int shift, label_array_t* lable_array, FILE* machine)
    {
    char curr_arg[15] = "POISON";
    int current_count = -1;
    sscanf(ptr+shift,"%s",curr_arg);
    label_joining( lable_array, curr_arg);
    current_count = label_find( lable_array, curr_arg);
    lable_array->data[current_count].addres = ftell(machine);
    }

int main()
    {
    reg_name registries[register_count];
    reg_init(registries);
    long count_sym_in = -1;
    char* main_buffer = nullptr;
    buffer_read( (char**) &main_buffer, &count_sym_in, source_name, 0, "rt" );
    long line_count = lines( main_buffer, count_sym_in);

    char where_create_machine[50] =  "" ;
    strcat( where_create_machine,  machine_file_directory);
    strcat( where_create_machine,  machine_file_name);
    FILE* machine = fopen(  where_create_machine , "wb");

    char curr_cmd[15] = "";
    int curr_num = -1;
    line* strings = nullptr;
    strings = (line*) calloc(line_count, sizeof(line));
    change(main_buffer,&count_sym_in, '\n','\0');
    fragmentation( strings, main_buffer, count_sym_in );

    int shift = 0;
    int current_arg = -1;
    //char curr_arg[15] = "POISON";
    //double double_arg = NAN;
    bool lable_list = true;

#define DEF_CMD(name,num,count_of_arguments,label,regist_ex,ram_ex,name_string,cmd_code)\
    if ( strcmp(name_string,curr_cmd) == 0 )\
        {\
        shifting( strings[i].begin_ptr, &shift, strlen(curr_cmd) );\
        if ( strcmp( "PUSH" ,curr_cmd ) == 0 )\
            {\
            pushargwork( strings[i].begin_ptr, &shift, &curr_num, machine );\
            continue;\
            }\
        curr_num = CMD__##name;\
        fwrite( &curr_num, sizeof(char), 1, machine);\
        if ( (count_of_arguments != 0)  )\
            {\
            for ( int j = 0 ; j < count_of_arguments ; j++ )\
                {\
                argwork( strings[i].begin_ptr, &shift, &current_arg, &lable_array, label, regist_ex, lable_list );\
                fwrite( &current_arg, sizeof(int), 1, machine);\
                }\
            continue;\
            }\
        continue;\
        }

    label_array_t lable_array ;
    label_array_init( &lable_array );

    for (int  i = 0; i < line_count ; i++)
        {
        shift = 0;
        shifting( strings[i].begin_ptr, &shift );
        if ( strchr(strings[i].begin_ptr+shift,':') != nullptr )
            {
            label_init( strings[i].begin_ptr, shift, &lable_array, machine );
            continue;
            }
        if ( *(strings[i].begin_ptr+shift) == '\0')
            {
            continue;
            }
        sscanf(strings[i].begin_ptr+shift,"%s",curr_cmd);
        #include "commands.h"
        abort();
        }

    fclose( machine );
    machine = fopen(  where_create_machine , "wb");
    char g[9] = "spirit!";
    g[7] = version;
    fwrite( g, sizeof( int ),2, machine );
    lable_list = false;

    for (int  i = 0; i < line_count ; i++)
        {
        shift = 0;
        shifting( strings[i].begin_ptr, &shift );
        if ( ( strchr(strings[i].begin_ptr+shift,':') != nullptr ) || ( *(strings[i].begin_ptr+shift) == '\0') )
            {
            continue;
            }
        sscanf(strings[i].begin_ptr+shift,"%s",curr_cmd);
        #include "commands.h"
        }

#undef DEF_CMD

    fclose(machine);
    free( lable_array.data );
    free( main_buffer );
    free( strings );
    return 0;
    }