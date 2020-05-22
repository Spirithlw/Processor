//commands.h is a list of commands for my own processor

#define cps cpu_stk
#define am accuracy_mul
#define arg(index) current_arg[index-1]
#define push(stack,arg) stack_push( &stack , arg )
#define pop(stack) stack_pop( &stack )
#define reg(num) registries[num].regist
#define regad(num) reg_adr(registries, num)

DEF_CMD(push, 1,1,0,1,0,"PUSH",push(cps,arg(1) ) )
DEF_CMD(pop,  2,1,0,1,0,"POP", reg(arg(1)) = pop(cps) )
DEF_CMD(add,  3,0,0,0,0,"ADD", push( cps, pop(cps) + pop(cps) ) )
DEF_CMD(sub,  4,0,0,0,0,"SUB", push( cps, pop(cps) - pop(cps) ) )
DEF_CMD(mul,  5,0,0,0,0,"MUL", push( cps, ( (long long) pop(cps) * pop(cps) ) / am  ) )
DEF_CMD(div,  6,0,0,0,0,"DIV", push( cps, round( ( (double) pop(cps) /  pop(cps)  ) * am )  ) )
DEF_CMD(sqrt, 7,0,0,0,0,"SQRT",push( cps, round( sqrt( ( (double) pop(cps) / am ) )*am) ) )
DEF_CMD(sin,  8,0,0,0,0,"SIN", push( cps, round( sin ( ( (double) pop(cps) / am ) )*am ) ) )
DEF_CMD(out,  9,0,0,0,0,"OUT", fprintf( out, "%g\n", (double) pop(cps) / am ) )

DEF_CMD(push_from_registr,10,1,0,1,0,"PUSH", push( cps, reg(   arg(1) ) ) )
DEF_CMD(push_from_ram,    11,2,0,0,1,"PUSH", push( cps, RAM[ ( arg(2) / am ) + regad( arg(1) ) ] ) )

DEF_CMD(jmp, 12,1,1,0,0,"JMP", pc = arg(1) )
DEF_CMD(end,  0,0,0,0,0,"END", pc = -1)

DEF_CMD(ja,  13,1,1,0,0,"JA",  if( pop(cps) >  pop(cps) ) { pc = arg(1); } )
DEF_CMD(jae, 14,1,1,0,0,"JAE", if( pop(cps) >= pop(cps) ) { pc = arg(1); } )
DEF_CMD(jb,  15,1,1,0,0,"JB",  if( pop(cps) <  pop(cps) ) { pc = arg(1); } )
DEF_CMD(jbe, 16,1,1,0,0,"JBE", if( pop(cps) <= pop(cps) ) { pc = arg(1); } )
DEF_CMD(je,  17,1,1,0,0,"JE",  if( pop(cps) == pop(cps) ) { pc = arg(1); } )
DEF_CMD(jne, 18,1,1,0,0,"JNE", if( pop(cps) != pop(cps) ) { pc = arg(1); } )

DEF_CMD(call,19,1,1,0,0,"CALL", push( ret_stk, pc) ; pc = arg(1) )
DEF_CMD(ret, 20,0,0,0,0,"RET",  pc = pop( ret_stk) )

DEF_CMD(push_to_ram,21,2,0,0,1,"PUSH", RAM[ ( arg(2) / am ) +  regad( arg(1) )  ] = pop(cps) )

#undef regad
#undef cps
#undef am
#undef arg
#undef push
#undef pop
#undef reg
