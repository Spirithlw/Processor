//commands.h is a list of commands for my own processor

#define cps cpu_stk
#define am accuracy_mul
#define arg(index) current_arg[index-1]
#define push(stack,arg) stack_push( &stack$ arg )
#define pop(stack) stack_pop( &stack )
#define reg(num) registries[num].regist
#define rarg(index) round( current_arg[index-1] / accuracy_mul )

DEF_CMD(push,1,1,0,0,0,"PUSH",stack_push(&cpu_stk $ current_arg[0]) )
DEF_CMD(pop,2,1,0,1,0,"POP",registries[current_arg[0]].regist = stack_pop( &cpu_stk ) )
DEF_CMD(add,3,0,0,0,0,"ADD", stack_push( &cpu_stk $ stack_pop( &cpu_stk ) + stack_pop( &cpu_stk )  ))
DEF_CMD(sub,4,0,0,0,0,"SUB",stack_push( &cpu_stk $ stack_pop( &cpu_stk ) - stack_pop( &cpu_stk )  ))
DEF_CMD(mul,5,0,0,0,0,"MUL",stack_push( &cpu_stk $ ( (long long) stack_pop( &cpu_stk ) * stack_pop( &cpu_stk ) ) / accuracy_mul  ))
DEF_CMD(div,6,0,0,0,0,"DIV",stack_push( &cpu_stk $ round( ( (double) stack_pop( &cpu_stk ) / accuracy_mul ) / ( (double) stack_pop( &cpu_stk )  / accuracy_mul )  * accuracy_mul )  ))
DEF_CMD(sqrt,7,0,0,0,0,"SQRT",stack_push( &cpu_stk $ round( sqrt(( (double) stack_pop( &cpu_stk) / accuracy_mul ) )*accuracy_mul) ) )
DEF_CMD(sin,8,0,0,0,0,"SIN",stack_push( &cpu_stk $ round( sin(( (double) stack_pop( &cpu_stk) / accuracy_mul ) )*accuracy_mul) ) )
DEF_CMD(out,9,0,0,0,0,"OUT",fprintf( out, "%g\n", (double) stack_pop(&cpu_stk) / accuracy_mul ) )
DEF_CMD(push_from_registr,10,1,0,1,0,"PUSH",stack_push( &cpu_stk $ registries[current_arg[0]].regist ) )
DEF_CMD(push_from_ram,11,2,0,0,1,"PUSH", stack_push( &cpu_stk $ RAM[ ( current_arg[1] / accuracy_mul )+registries[current_arg[0]].regist] ) )
DEF_CMD(jmp,12,1,1,0,0,"JMP",pc = round( current_arg[0] / accuracy_mul ) )
DEF_CMD(end,0,0,0,0,0,"END",pc = -1)
DEF_CMD(ja,13,1,1,0,0,"JA", if( stack_pop(&cpu_stk) > stack_pop(&cpu_stk) ) { pc = round( current_arg[0] / accuracy_mul ); } )
DEF_CMD(jae,14,1,1,0,0,"JAE", if( stack_pop(&cpu_stk) >= stack_pop(&cpu_stk) ) { pc = round( current_arg[0] / accuracy_mul ); } )
DEF_CMD(jb,15,1,1,0,0,"JB", if( stack_pop(&cpu_stk) < stack_pop(&cpu_stk) ) { pc = round( current_arg[0] / accuracy_mul ); } )
DEF_CMD(jbe,16,1,1,0,0,"JBE", if( stack_pop(&cpu_stk) <= stack_pop(&cpu_stk) ) { pc = round( current_arg[0] / accuracy_mul ); } )
DEF_CMD(je,17,1,1,0,0,"JE", if( stack_pop(&cpu_stk) == stack_pop(&cpu_stk) ) { pc = round( current_arg[0] / accuracy_mul ); } )
DEF_CMD(jme,18,1,1,0,0,"JNE", if( stack_pop(&cpu_stk) != stack_pop(&cpu_stk) ) { pc = round( current_arg[0] / accuracy_mul ); } )
DEF_CMD(call,19,1,1,0,0,"CALL", stack_push( &ret_stk, pc) ; pc = round( current_arg[0] / accuracy_mul ) )
DEF_CMD(ret,20,0,0,0,0,"RET", pc = stack_pop( &ret_stk ) )
DEF_CMD(push_to_ram,21,2,0,0,1,"PUSH", RAM[ ( current_arg[1] / accuracy_mul ) + ( registries[ current_arg[0] ].regist ) / accuracy_mul ] = stack_pop( &cpu_stk ) )

#undef cps
#undef am
#undef arg
#undef push
#undef pop
#undef reg
#undef rarg