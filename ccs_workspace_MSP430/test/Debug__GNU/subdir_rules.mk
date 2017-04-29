################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"/home/saurav/ti/energia-0101E0017/hardware/tools/msp430/bin/msp430-gcc" -c -mcpu=430x -mmcu=msp430f5229 -I"/home/saurav/ti/ccsv6/ccs_base/msp430/include_gcc" -I"/home/saurav/ti/energia-0101E0017/hardware/tools/msp430/msp430/include" -Os -g -gdwarf-3 -gstrict-dwarf -Wall -mlarge -mhwmult=f5series -mcode-region=none -mdata-region=none -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


