#ifndef _TEST_EXPRESSION_PARSER_H_
#define _TEST_EXPRESSION_PARSER_H_

struct SAMPLE_EXPRESSION
{
  int         id;
  GXLPCSTR    szSourceFile;
  GXINT       nLine;
  const char* expression;
  int         expectation;
  GXLPCSTR*   aOperStack;
  GXBOOL      bDbgBreak;    // �����ж�
};

#endif // _TEST_EXPRESSION_PARSER_H_