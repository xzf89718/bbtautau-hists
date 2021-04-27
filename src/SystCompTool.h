#ifndef SYSTCOMPTOOL_H
#define SYSTCOMPTOOL_H

#include "CompTool.h"

/**
 * @note: the differece it that this tool focus on one process only
 *        with many systemtics, while the CompTool can do more things,
 *        e.g. compare systematics and other samples at the same time.
 *        Maybe they can be merged into one.. I don't know.
 */
class SystCompTool : public CompTool
{
public:
  explicit SystCompTool(const CompInfo* info) : CompTool(info), m_info(info) {}
  virtual ~SystCompTool() override {}

public:
  virtual bool check(const Config* c) const override;
  // colorful
  virtual void paint(const Config* c) const override;
  // drawing
  virtual void run(const Config* c) const override;

public:
  // uncertainty obtained for PDF4LHC Hessian sets
  // ref: https://arxiv.org/pdf/1510.03865.pdf
  void uncHessianPDF4LHC(const Config* c) const;

protected:
  const CompInfo* m_info;

};

#endif