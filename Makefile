#makefile for vcimp

RELIC_PTH = /usr/local
GMP_PTH = /usr/local
FLINT_PTH = /usr/local

DIR_PTH = .
IDIR = include
ODIR = obj
SDIR = src

CC=gcc

#Utils path
_UTIL_PTH = utils
_TEST_PTH = test
IUTIL_PTH = $(DIR_PTH)/$(IDIR)/$(_UTIL_PTH)
OUTIL_PTH = $(DIR_PTH)/$(ODIR)/$(_UTIL_PTH)
SUTIL_PTH = $(DIR_PTH)/$(SDIR)/$(_UTIL_PTH)

# Pi1 scheme paths
_PI1_PTH = pi1
IPI1_PTH = $(DIR_PTH)/$(IDIR)/$(_PI1_PTH)
OPI1_PTH = $(DIR_PTH)/$(ODIR)/$(_PI1_PTH)
SPI1_PTH = $(DIR_PTH)/$(SDIR)/$(_PI1_PTH)

# Pi2 scheme paths
_PI2_PTH = pi2
IPI2_PTH = $(DIR_PTH)/$(IDIR)/$(_PI2_PTH)
OPI2_PTH = $(DIR_PTH)/$(ODIR)/$(_PI2_PTH)
SPI2_PTH = $(DIR_PTH)/$(SDIR)/$(_PI2_PTH)

# Pi3 scheme paths
_PI3_PTH = pi3
IPI3_PTH = $(DIR_PTH)/$(IDIR)/$(_PI3_PTH)
OPI3_PTH = $(DIR_PTH)/$(ODIR)/$(_PI3_PTH)
SPI3_PTH = $(DIR_PTH)/$(SDIR)/$(_PI3_PTH)

# Mvm scheme paths
_MVM_PTH = mvm
IMVM_PTH = $(DIR_PTH)/$(IDIR)/$(_MVM_PTH)
OMVM_PTH = $(DIR_PTH)/$(ODIR)/$(_MVM_PTH)
SMVM_PTH = $(DIR_PTH)/$(SDIR)/$(_MVM_PTH)

# MM42 scheme paths
_MM42_PTH = mm42
IMM42_PTH = $(DIR_PTH)/$(IDIR)/$(_MM42_PTH)
OMM42_PTH = $(DIR_PTH)/$(ODIR)/$(_MM42_PTH)
SMM42_PTH = $(DIR_PTH)/$(SDIR)/$(_MM42_PTH)

# VDMF scheme paths
_VDMF_PTH = vdmf
IVDMF_PTH = $(DIR_PTH)/$(IDIR)/$(_VDMF_PTH)
OVDMF_PTH = $(DIR_PTH)/$(ODIR)/$(_VDMF_PTH)
SVDMF_PTH = $(DIR_PTH)/$(SDIR)/$(_VDMF_PTH)

# PE33 scheme paths
_PE33_PTH = pe33
IPE33_PTH = $(DIR_PTH)/$(IDIR)/$(_PE33_PTH)
OPE33_PTH = $(DIR_PTH)/$(ODIR)/$(_PE33_PTH)
SPE33_PTH = $(DIR_PTH)/$(SDIR)/$(_PE33_PTH)


# Test examples paths
OTEST_PTH=test/obj
STEST_PTH=test

# Libraries
LIB_PTH = -L$(GMP_PTH)/lib -L$(RELIC_PTH)/lib -L$(FLINT_PTH)/lib
LIBS = -lgmp -lrelic_s -lrelic -lflint 
LFLAGS = $(LIB_PTH) $(LIBS) 

# Dependencies

# Libraries
_GMP_DEPS = gmp.h
_RELIC_DEPS = relic.h
_FLINT_DEPS = fmpz.h fmpz_poly.h fq.h fq_poly.h

GMP_DEPS = $( patsubst %, $(GMP_PTH)$(IDIR)/%, $(_GMP_DEPS) )
RELIC_DEPS = $( patsubst %, $(RELIC_PTH)$(IDIR)/%, $(_RELIC_DEPS) )
FLINT_DEPS = $( patsubst %, $(FLINT_PTH)$(IDIR)/flint/%, $(_FLINT_DEPS) )

# VCIMP utils deps
_UTIL_DEPS = utils.h hash.h lhe.h lhep.h  
UTIL_DEPS = $( patsubst %, $(IUTIL_PTH)/%, $(_UTIL_DEPS) )

# PI1 scheme deps
_PI1_DEPS = cgpp.h pi1.h
PI1_DEPS = $( patsubst %, $(IPI1_PTH)/%, $(_PI1_DEPS) )


# PI2 scheme deps
_PI2_DEPS = pi2.h
PI2_DEPS = $( patsubst %, $(IPI2_PTH)/%, $(_PI2_DEPS) )

# PI3 scheme deps
_PI3_DEPS = pi3.h
PI3_DEPS = $( patsubst %, $(IPI3_PTH)/%, $(_PI3_DEPS) )

# MVM scheme deps
_MVM_DEPS = mvm.h mvmp.h prfm.h
MVM_DEPS = $( patsubst %, $(IMVM_PTH)/%, $(_MVM_DEPS) )

# MM42 scheme deps
_MM42_DEPS = mm42.h param.h
MM42_DEPS = $( patsubst %, $(IMM42_PTH)/%, $(_MM42_DEPS) )

# VDMF scheme deps
_VDMF_DEPS = mf.h prf.h hash2.h
VDMF_DEPS = $( patsubst %, $(IVDMF_PTH)/%, $(_VDMF_DEPS) )

# PE33 scheme deps
_PE33_DEPS = poly_eval.h pep.h
PE33_DEPS = $( patsubst %, $(IPE33_PTH)/%, $(_PE33_DEPS) )

LIB_DEPS = $(UTIL_DEPS) $(PI1_DEPS)

DEPS = $(LIB_DEPS) $(GMP_DEPS) $(RELIC_DEPS) $(FLINT_DEPS) 

IFLAGS = -I$(GMP_PTH)/$(IDIR) -I$(FLINT_PTH)/$(IDIR)/flint -I$(RELIC_PTH)/$(IDIR)   -I$(IUTIL_PTH) 

# Object files

# test objects
_OBJ_PI1_TEST = pi1_test.o
OBJ_PI1_TEST = $(patsubst %,$(OTEST_PTH)/%,$(_OBJ_PI1_TEST))

_OBJ_PI2_TEST = pi2_test.o
OBJ_PI2_TEST = $(patsubst %,$(OTEST_PTH)/%,$(_OBJ_PI2_TEST))

_OBJ_PI3_TEST = pi3_test.o
OBJ_PI3_TEST = $(patsubst %,$(OTEST_PTH)/%,$(_OBJ_PI3_TEST))

_OBJ_MVM_TEST = mvm_test.o
OBJ_MVM_TEST = $(patsubst %,$(OTEST_PTH)/%,$(_OBJ_MVM_TEST))

_OBJ_MM42_TEST = mm42_test.o
OBJ_MM42_TEST = $(patsubst %,$(OTEST_PTH)/%,$(_OBJ_MM42_TEST))

_OBJ_VDMF_TEST = vdmf_test.o
OBJ_VDMF_TEST = $(patsubst %,$(OTEST_PTH)/%,$(_OBJ_VDMF_TEST))

_OBJ_PE33_TEST = pe33_test.o
OBJ_PE33_TEST = $(patsubst %,$(OTEST_PTH)/%,$(_OBJ_PE33_TEST))


#VCIMP utils objects
_OBJ_UTIL = utils.o hash.o lhe.o lhep.o
OBJ_UTIL = $(patsubst %,$(OUTIL_PTH)/%,$(_OBJ_UTIL))

# PI1 scheme objects
_OBJ_PI1 = cgpp.o pi1.o
OBJ_PI1 = $(patsubst %,$(OPI1_PTH)/%,$(_OBJ_PI1))

# PI2 scheme objects
_OBJ_PI2 = pi2.o
OBJ_PI2 = $(patsubst %,$(OPI2_PTH)/%,$(_OBJ_PI2))

# PI2 scheme objects
_OBJ_PI3 = pi3.o
OBJ_PI3 = $(patsubst %,$(OPI3_PTH)/%,$(_OBJ_PI3))

# MVM scheme objects
_OBJ_MVM = mvm.o mvmp.o prfm.o
OBJ_MVM = $(patsubst %,$(OMVM_PTH)/%,$(_OBJ_MVM))

# MM42 scheme objects
_OBJ_MM42 = mm42.o param.o
OBJ_MM42 = $(patsubst %,$(OMM42_PTH)/%,$(_OBJ_MM42))

# VDMF scheme objects
_OBJ_VDMF = mf.o prf.o hash2.o
OBJ_VDMF = $(patsubst %,$(OVDMF_PTH)/%,$(_OBJ_VDMF))

# PE33 scheme objects
_OBJ_PE33 = poly_eval.o pep.o
OBJ_PE33 = $(patsubst %,$(OPE33_PTH)/%,$(_OBJ_PE33))

all: pi1_test pi2_test pi3_test mvm_test mm42_test vdmf_test pe33_test

pi1_test: $(OBJ_PI1_TEST) $(OBJ_PI1) $(OBJ_UTIL)
	cc -o pi1_test $(OBJ_PI1_TEST) $(OBJ_PI1) $(OBJ_UTIL) -L/lib $(IFLAGS)-I$(IPI1_PTH) $(LFLAGS)

pi2_test: $(OBJ_PI2_TEST) $(OBJ_PI2) $(OBJ_UTIL)
	cc -o pi2_test $(OBJ_PI2_TEST) $(OBJ_PI2) $(OBJ_UTIL) -L/lib $(IFLAGS)-I$(IPI2_PTH) $(LFLAGS)

pi3_test: $(OBJ_PI3_TEST) $(OBJ_PI3) $(OBJ_UTIL)
	cc -o pi3_test $(OBJ_PI3_TEST) $(OBJ_PI3) $(OBJ_UTIL) -L/lib $(IFLAGS)-I$(IPI3_PTH) $(LFLAGS)

mvm_test: $(OBJ_MVM_TEST) $(OBJ_MVM) $(OBJ_UTIL)
	cc -g -o mvm_test $(OBJ_MVM_TEST) $(OBJ_MVM) $(OBJ_UTIL) -L/lib $(IFLAGS)-I$(IMVM_PTH) $(LFLAGS)

mm42_test: $(OBJ_MM42_TEST) $(OBJ_MM42) $(OBJ_UTIL)
	cc -o mm42_test $(OBJ_MM42_TEST) $(OBJ_MM42) $(OBJ_UTIL) -L/lib $(IFLAGS)-I$(IMM42_PTH) $(LFLAGS)

vdmf_test: $(OBJ_VDMF_TEST) $(OBJ_VDMF) $(OBJ_UTIL)
	cc -o vdmf_test $(OBJ_VDMF_TEST) $(OBJ_VDMF) $(OBJ_UTIL) -L/lib $(IFLAGS)-I$(IVDMF_PTH) $(LFLAGS)  

pe33_test: $(OBJ_PE33_TEST) $(OBJ_PE33) $(OBJ_UTIL)
	cc -o pe33_test $(OBJ_PE33_TEST) $(OBJ_PE33) $(OBJ_UTIL) -L/lib $(IFLAGS)-I$(IPE33_PTH) $(LFLAGS) 

$(OUTIL_PTH)/%.o: $(SUTIL_PTH)/%.c $(DEPS) |$(OUTIL_PTH)
	cc -c -o $@ $< $(IFLAGS) $(LFLAGS)

$(OPI1_PTH)/%.o: $(SPI1_PTH)/%.c $(DEPS) |$(OPI1_PTH)
	cc -c -o $@ $< $(IFLAGS)-I$(IPI1_PTH) $(LFLAGS)

$(OTEST_PTH)/pi1_test.o: $(STEST_PTH)/pi1_test.c |$(OTEST_PTH)
	cc -c -o $@ $< $(IFLAGS)-I$(IPI1_PTH) $(LFLAGS)


$(OPI2_PTH)/%.o: $(SPI2_PTH)/%.c $(DEPS) |$(OPI2_PTH)
	cc -c -o $@ $< $(IFLAGS)-I$(IPI2_PTH) $(LFLAGS)

$(OTEST_PTH)/pi2_test.o: $(STEST_PTH)/pi2_test.c |$(OTEST_PTH)
	cc -c -o $@ $< $(IFLAGS)-I$(IPI2_PTH) $(LFLAGS)


$(OPI3_PTH)/%.o: $(SPI3_PTH)/%.c $(DEPS) |$(OPI3_PTH)
	cc -c -o $@ $< $(IFLAGS)-I$(IPI3_PTH) $(LFLAGS)

$(OTEST_PTH)/pi3_test.o: $(STEST_PTH)/pi3_test.c |$(OTEST_PTH)
	cc -c -o $@ $< $(IFLAGS)-I$(IPI3_PTH) $(LFLAGS)

$(OMVM_PTH)/%.o: $(SMVM_PTH)/%.c $(DEPS) |$(OMVM_PTH)
	cc -c -o $@ $< $(IFLAGS)-I$(IMVM_PTH) $(LFLAGS)

$(OTEST_PTH)/mvm_test.o: $(STEST_PTH)/mvm_test.c |$(OTEST_PTH)
	cc -c -o $@ $< $(IFLAGS)-I$(IMVM_PTH) $(LFLAGS)

$(OMM42_PTH)/%.o: $(SMM42_PTH)/%.c $(DEPS) |$(OMM42_PTH)
	cc -c -o $@ $< $(IFLAGS)-I$(IMM42_PTH) $(LFLAGS)

$(OTEST_PTH)/mm42_test.o: $(STEST_PTH)/mm42_test.c |$(OTEST_PTH)
	cc -c -o $@ $< $(IFLAGS)-I$(IMM42_PTH) $(LFLAGS)

$(OVDMF_PTH)/%.o: $(SVDMF_PTH)/%.c $(DEPS) |$(OVDMF_PTH)
	cc -c -o $@ $< $(IFLAGS)-I$(IVDMF_PTH) $(LFLAGS)

$(OTEST_PTH)/vdmf_test.o: $(STEST_PTH)/vdmf_test.c |$(OTEST_PTH)
	cc -c -o $@ $< $(IFLAGS)-I$(IVDMF_PTH) $(LFLAGS)

$(OPE33_PTH)/%.o: $(SPE33_PTH)/%.c $(DEPS) |$(OPE33_PTH)
	cc -c -o $@ $< $(IFLAGS)-I$(IPE33_PTH) $(LFLAGS)

$(OTEST_PTH)/pe33_test.o: $(STEST_PTH)/pe33_test.c |$(OTEST_PTH)
	cc -c -o $@ $< $(IFLAGS)-I$(IPE33_PTH) $(LFLAGS)

$(OUTIL_PTH):
	mkdir -p $@

$(OTEST_PTH):
	mkdir -p $@

$(OPI1_PTH):
	mkdir -p $@

$(OPI2_PTH):
	mkdir -p $@

$(OPI3_PTH):
	mkdir -p $@

$(OMVM_PTH):
	mkdir -p $@

$(OMM42_PTH):
	mkdir -p $@

$(OVDMF_PTH):
	mkdir -p $@

$(OPE33_PTH):
	mkdir -p $@

.PHONY : clean
 clean:
	rm -f $(OTEST_PTH)/*.o $(OUTIL_PTH)/*.o  $(OPI1_PTH)/*.o $(OPI2_PTH)/*.o $(OPI3_PTH)/*.o $(OMVM_PTH)/*.o $(OMM42_PTH)/*.o $(OVDMF_PTH)/*.o $(OPE33_PTH)/*.o
	rm -f pi1_test pi2_test pi3_test mvm_test mm42_test vdmf_test pe33_test
	
