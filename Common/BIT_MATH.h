#ifndef BIT_MATH_H_
#define BIT_MATH_H_


//bit Math
#define SET_BIT(REG,BIT) REG|=(1<<BIT)
#define CLR_BIT(REG,BIT) REG&=~(1<<BIT)
#define TGL_BIT(REG,BIT) REG^=(1<<BIT)
#define GET_BIT(REG,BIT) ((REG>>BIT)&1)

#define ASSIGN_BIT(REG,BIT_NO,VAL) (REG=(REG&(~(1<<BIT_NO)))|(VAL<<BIT_NO))


#endif 