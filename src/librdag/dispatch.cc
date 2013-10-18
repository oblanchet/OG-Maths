/*
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * See distribution for licence.
 *
 */

#include "dispatch.hh"
#include "expression.hh"
#include "terminal.hh"
#include "warningmacros.h"
#include "uncopyable.hh"
#include <iostream>

namespace librdag {

template class DispatchUnaryOp<void>;
template class DispatchBinaryOp<void>;

void PlusRunner::run(RegContainer SUPPRESS_UNUSED * reg0, OGComplexMatrix const SUPPRESS_UNUSED * arg0, OGComplexMatrix const SUPPRESS_UNUSED * arg1) const
{

}

void PlusRunner::run(RegContainer SUPPRESS_UNUSED * reg0, OGRealMatrix const SUPPRESS_UNUSED * arg0, OGRealMatrix const SUPPRESS_UNUSED * arg1) const
{
  
}

void PlusRunner::run(RegContainer* reg0, OGRealScalar const * arg0, OGRealScalar const * arg1) const
{
  // impl convert and run for types OGRealScalar and OGRealScalar 
  cout << "In virtual overridden PlusRunner T run() REAL REAL " << std::endl;
  const OGRealScalar * ret = new OGRealScalar(arg0->getValue()+arg1->getValue());
  reg0->push_back(ret);
}
  
void
NegateRunner::run(RegContainer* reg, const OGRealScalar* arg) const
{
  cout << "IN Negate runner." << endl;
  const OGRealScalar* ret = new OGRealScalar(-(arg->getValue()));
  reg->push_back(ret);
}

void
NegateRunner::run(RegContainer *reg, const OGRealMatrix *arg) const
{
  real16* data = arg->getData();
  int datalen = arg->getDatalen();
  real16* newData = new real16[datalen];
  for (int i = 0; i < datalen; ++i)
  {
    newData[i] = -data[i];
  }
  reg->push_back(new OGRealMatrix(newData, arg->getRows(), arg->getCols()));
}

void
NegateRunner::run(RegContainer SUPPRESS_UNUSED *reg, const OGComplexMatrix SUPPRESS_UNUSED *arg) const
{
  complex16* data = arg->getData();
  int datalen = arg->getDatalen();
  complex16* newData = new complex16[datalen];
  for (int i = 0; i < datalen; ++i)
  {
    newData[i] = -data[i];
  }
  reg->push_back(new OGComplexMatrix(data, arg->getRows(), arg->getCols()));
}

Dispatcher::Dispatcher()
{
    _PlusRunner = new PlusRunner();
    _NegateRunner = new NegateRunner();
}
  
  
Dispatcher::~Dispatcher(){
  delete _PlusRunner;
  delete _NegateRunner;
}
  
void Dispatcher::dispatch(OGNumeric const *thing) const
{
    cout << "Dispatching..." << std::endl;
    ExprType_t ID = thing->getType();
    cout << "TYPE IS" << ID << std::endl;
    bool isTerminalType = false;
    if(ID&IS_NODE_MASK){
      cout << "is node..." << std::endl;
      thing->debug_print();
      thing->asOGExpr();
    } else {
      isTerminalType = true;      
      cout << "is terminal..." << std::endl;
      thing->debug_print();       
    }

    // branch switch on isTerminalType?
    if(isTerminalType) {
      switch(ID)
      {
        case REAL_SCALAR_ENUM:
          dispatch(thing->asOGRealScalar());
          break;
        case COMPLEX_SCALAR_ENUM:
          dispatch(thing->asOGComplexScalar());
          break;
        case REAL_MATRIX_ENUM:
          dispatch(thing->asOGRealMatrix());
          break;
        case COMPLEX_MATRIX_ENUM:
          dispatch(thing->asOGComplexMatrix());
          break;
        case REAL_SPARSE_MATRIX_ENUM:
          dispatch(thing->asOGRealSparseMatrix());
          break;
        case COMPLEX_SPARSE_MATRIX_ENUM:
          dispatch(thing->asOGComplexSparseMatrix());
          break;
        case REAL_DIAGONAL_MATRIX_ENUM:
          dispatch(thing->asOGRealDiagonalMatrix());
          break;
        case COMPLEX_DIAGONAL_MATRIX_ENUM:
          dispatch(thing->asOGComplexDiagonalMatrix());
          break;
        case LOGICAL_MATRIX_ENUM:
          dispatch(thing->asOGRealMatrix()); // note dispatch type as base class T
          break;
        case INTEGER_SCALAR_ENUM:
          dispatch(thing->asOGIntegerScalar());
          break;          
        default:
          cout << "NO SPECIFIC TYPE " << std::endl;
      }     
    }
    else
    {
      switch(ID)
      {
        case PLUS_ENUM:
          dispatch(thing->asPLUS());
          break;
        case NEGATE_ENUM:
          dispatch(thing->asNEGATE());
          break;
        case COPY_ENUM:
          dispatch(thing->asCOPY());
          break;
        case SVD_ENUM:
          dispatch(thing->asSVD());
          break;
        case SELECTRESULT_ENUM:
          dispatch(thing->asSELECTRESULT());
          break;
        default:
          cout << "NO SPECIFIC TYPE " << std::endl;
      }
    }
}
  
// specific terminal dispatches
void Dispatcher::dispatch(OGRealScalar const SUPPRESS_UNUSED * thing) const {

}

void Dispatcher::dispatch(OGComplexScalar const SUPPRESS_UNUSED * thing) const {
  
}

void Dispatcher::dispatch(OGIntegerScalar const SUPPRESS_UNUSED * thing) const {
  
}

void Dispatcher::dispatch(OGRealMatrix const SUPPRESS_UNUSED * thing) const {
  
}

void Dispatcher::dispatch(OGLogicalMatrix const SUPPRESS_UNUSED * thing) const {
  
}

void Dispatcher::dispatch(OGComplexMatrix const SUPPRESS_UNUSED * thing) const {
  
}

void Dispatcher::dispatch(OGRealDiagonalMatrix const SUPPRESS_UNUSED * thing) const {
  
}

void Dispatcher::dispatch(OGComplexDiagonalMatrix const SUPPRESS_UNUSED * thing) const {
  
}

void Dispatcher::dispatch(OGRealSparseMatrix const SUPPRESS_UNUSED * thing) const {
  
}

void Dispatcher::dispatch(OGComplexSparseMatrix const SUPPRESS_UNUSED * thing) const {
  
}
  
void Dispatcher::dispatch(PLUS const *thing) const {
      cout << "ABOUT TO DISPATCH A PLUS OP" << std::endl;
      const ArgContainer * args = thing->getArgs();
      const RegContainer * regs = thing->getRegs();
      const OGNumeric * arg0 = (*args)[0];
      const OGNumeric * arg1 = (*args)[1];
      const OGTerminal* arg0t = arg0->asOGTerminal();
      const OGTerminal* arg1t = arg1->asOGTerminal();
      if (arg0t == nullptr)
      {
        arg0t = (*(arg0->asOGExpr()->getRegs()))[0]->asOGTerminal();
      }
      if (arg1t == nullptr)
      {
        arg1t = (*(arg1->asOGExpr()->getRegs()))[0]->asOGTerminal();
      }
      this->_PlusRunner->eval(const_cast<RegContainer *>(regs), arg0t, arg1t);
}

void Dispatcher::dispatch(NEGATE const SUPPRESS_UNUSED *thing) const {
      cout << "ABOUT TO DISPATCH A NEGATE OP" << std::endl;
      const ArgContainer* args = thing->getArgs();
      const RegContainer* regs = thing->getRegs();
      const OGNumeric *arg = (*args)[0];
      _NegateRunner->eval(const_cast<RegContainer *>(regs), arg->asOGTerminal());
}

void Dispatcher::dispatch(COPY const SUPPRESS_UNUSED *thing) const {
      cout << "ABOUT TO DISPATCH A COPY OP" << std::endl;
}

void Dispatcher::dispatch(SVD const SUPPRESS_UNUSED *thing) const {
      cout << "ABOUT TO DISPATCH A SVD OP" << std::endl;
}

void Dispatcher::dispatch(SELECTRESULT const SUPPRESS_UNUSED *thing) const {
      cout << "ABOUT TO DISPATCH A SELECTRESULT OP" << std::endl;
}
  
  

} // namespace librdag


