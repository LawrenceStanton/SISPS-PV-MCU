set(CPU -mcpu=cortex-m0plus)
set(FPU )
set(FLOAT_ABI )

set(CPU_PARAMETERS 
	${CPU}
	-mthumb
	${FPU}
	${FLOAT_ABI}
)
