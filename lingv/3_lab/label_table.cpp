/*
  LabelTable.cpp 
  ћодуль, отвечающий за хранение меток перехода
*/ 

#include "stdafx.h"
#include "label_table.h"


class LabelTableImpl {
  std::map<unsigned int, Parser::Holder> labels;
public:
  void           AddLabel(unsigned int, Parser::Holder);
  Parser::Holder GetHolder(unsigned int);
};


LabelTable::LabelTable() : pImpl(new LabelTableImpl) { }
LabelTable::~LabelTable() { delete pImpl; pImpl=0; }
void LabelTable::AddLabel(unsigned int param1, Parser::Holder param2) {
  pImpl->AddLabel(param1,param2);
}
Parser::Holder LabelTable::GetHolder(unsigned int param) {
  return pImpl->GetHolder(param);
}

void LabelTableImpl::AddLabel(unsigned int label, Parser::Holder holder) {
  labels[label] = holder;
}

Parser::Holder LabelTableImpl::GetHolder(unsigned int label)  {
  if ( 0 == labels.count(label) )
    throw "Label is absent";
  return labels[label];
}

LabelTable LT;