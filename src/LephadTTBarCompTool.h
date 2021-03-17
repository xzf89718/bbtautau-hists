#ifndef LEPHADTTBARCOMPTOOL_H
#define LEPHADTTBARCOMPTOOL_H

#include "CompTool.h"

class LephadTTBarCompTool : public CompTool
{
public:
  explicit LephadTTBarCompTool(const CompInfo* info) : CompTool(info), m_info(info) {}
  virtual ~LephadTTBarCompTool() override {}

public:
  // colorful
  virtual void paint(const Config* c) const override;

public:
  // add in quadrature
  /// @todo maybe share some common functionality
  void combineReweightSysts(const Config* c); 

protected:
  bool m_isCombined = false;
  const CompInfo* m_info;
};

#endif