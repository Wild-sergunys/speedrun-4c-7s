/*
  Commands.h
  Интерфейс команд языка
*/ 

#pragma once 
#include "interface.h"

class CmdExpression : public Command {
public: 
  virtual bool Process(Parser&);
};

class CmdPrint : public Command {
public: 
  virtual bool Process(Parser&);
};

class CmdInput : public Command {
public: 
  virtual bool Process(Parser&);
};

class CmdFor : public Command {
public: 
  virtual bool Process(Parser&);
};

class CmdNext : public Command {
public: 
  virtual bool Process(Parser&);
};

class CmdGoto : public Command {
public: 
  virtual bool Process(Parser&);
};

class CmdIf : public Command {
public: 
  virtual bool Process(Parser&);
};