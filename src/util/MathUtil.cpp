#include "sbmlsim/internal/util/MathUtil.h"
#include <cmath>

const unsigned long long FACTORIAL_TABLE[] = { // size 20
    0, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600, 6227020800,
    87178291200, 1307674368000, 20922789888000, 355687428096000, 6402373705728000, 121645100408832000
    // 2432902008176640000, 51090942171709440000, 1124000727777607680000, 25852016738884976640000,
    // 620448401733239439360000, 15511210043330985984000000, 403291461126605635584000000,
    // 10888869450418352160768000000, 304888344611713860501504000000, 8841761993739701954543616000000,
    // 265252859812191058636308480000000, 8222838654177922817725562880000000, 263130836933693530167218012160000000,
    // 8683317618811886495518194401280000000, 295232799039604140847618609643520000000,
    // 10333147966386144929666651337523200000000, 371993326789901217467999448150835200000000,
    // 13763753091226345046315979581580902400000000, 523022617466601111760007224100074291200000000
};

double MathUtil::factorial(unsigned long long n) {
  if (n <= 19) {
    return FACTORIAL_TABLE[n];
  }

  double ret = FACTORIAL_TABLE[19];
  for (unsigned long long i = 20; i <= n; i++) {
    ret *= (double) i;
  }
  return ret;
}

long long MathUtil::ceil(double f) {
  return ::ceil(f);
}

/*
ASTNode* MathUtil::differentiate(const ASTNode *ast, std::string target) {
  ASTNodeType_t type = ast->getType();
  ASTNode *tmp = new ASTNode();
  ASTNode *tmp2, *tmp3;
  ASTNode *left, *right;

  if (containsTarget(ast, target) == 0) {
    ASTNode_setType(tmp, AST_INTEGER);
    ASTNode_setInteger(tmp, 0);
    return tmp;
  }

  switch (type) {
    case AST_PLUS :
      ASTNode_setType(tmp, AST_PLUS);
      ASTNode_addChild(tmp, differentiate(ASTNode_getLeftChild(ast), target));
      ASTNode_addChild(tmp, differentiate(ASTNode_getRightChild(ast), target));
      break;
    case AST_MINUS :
      ASTNode_setType(tmp, AST_MINUS);
      ASTNode_addChild(tmp, differentiate(ASTNode_getLeftChild(ast), target));
      ASTNode_addChild(tmp, differentiate(ASTNode_getRightChild(ast), target));
      break;
    case AST_TIMES :
      ASTNode_setType(tmp, AST_PLUS);
      left = ASTNode_createWithType(AST_TIMES);
      ASTNode_addChild(left, differentiate(ASTNode_getLeftChild(ast), target));
      ASTNode_addChild(left, ASTNode_deepCopy(ASTNode_getRightChild(ast)));
      ASTNode_addChild(tmp, left);
      right = ASTNode_createWithType(AST_TIMES);
      ASTNode_addChild(right, ASTNode_deepCopy(ASTNode_getLeftChild(ast)));
      ASTNode_addChild(right, differentiate(ASTNode_getRightChild(ast), target));
      ASTNode_addChild(tmp, right);
      break;
    case AST_DIVIDE :
      ASTNode_setType(tmp, AST_DIVIDE);
      if (containsTarget(ASTNode_getRightChild(ast), target) == 0) {
        ASTNode_addChild(tmp, differentiate(ASTNode_getLeftChild(ast),target));
        ASTNode_addChild(tmp, ASTNode_deepCopy(ASTNode_getRightChild(ast)));
        break;
      }
      left = ASTNode_createWithType(AST_MINUS);
      ASTNode_t *ll = ASTNode_createWithType(AST_TIMES);
      ASTNode_t *lr = ASTNode_createWithType(AST_TIMES);
      ASTNode_addChild(ll, differentiate(ASTNode_getLeftChild(ast), target));
      ASTNode_addChild(ll, ASTNode_deepCopy(ASTNode_getRightChild(ast)));
      ASTNode_addChild(lr, ASTNode_deepCopy(ASTNode_getLeftChild(ast)));
      ASTNode_addChild(lr, differentiate(ASTNode_getRightChild(ast), target));
      ASTNode_addChild(left, ll);
      ASTNode_addChild(left, lr);
      ASTNode_addChild(tmp, left);
      right = ASTNode_createWithType(AST_FUNCTION_POWER);
      ASTNode_addChild(right, ASTNode_deepCopy(ASTNode_getRightChild(ast)));
      tmp2 = ASTNode_createWithType(AST_INTEGER);
      ASTNode_setInteger(tmp2, 2);
      ASTNode_addChild(right, tmp2);
      ASTNode_addChild(tmp, right);
      break;
    case AST_FUNCTION_POWER :
    case AST_POWER :
      ASTNode_setType(tmp, AST_TIMES);
      left = ASTNode_createWithType(AST_TIMES);
      ASTNode_addChild(left, ASTNode_deepCopy(ASTNode_getRightChild(ast)));
      ASTNode_addChild(left, differentiate(ASTNode_getLeftChild(ast), target));
      ASTNode_addChild(tmp, left);
      tmp3 = ASTNode_createWithType(AST_FUNCTION_POWER);
      ASTNode_addChild(tmp3, ASTNode_deepCopy(ASTNode_getLeftChild(ast)));
      right = ASTNode_createWithType(AST_MINUS);
      ASTNode_addChild(right, ASTNode_deepCopy(ASTNode_getRightChild(ast)));
      tmp2 = ASTNode_createWithType(AST_INTEGER);
      ASTNode_setInteger(tmp2, 1);
      ASTNode_addChild(right, tmp2);
      ASTNode_addChild(tmp3, right);
      ASTNode_addChild(tmp, tmp3);
      break;
    case AST_FUNCTION_ROOT :
      ASTNode_setType(tmp, AST_TIMES);
      ASTNode_addChild(tmp, differentiate(ASTNode_getLeftChild(ast), target));
      right = ASTNode_createWithType(AST_TIMES);
      tmp2 = ASTNode_createWithType(AST_REAL);
      ASTNode_setReal(tmp2, 0.5);
      ASTNode_addChild(right, tmp2);
      tmp2 = ASTNode_createWithType(AST_FUNCTION_POWER);
      ASTNode_addChild(tmp2, ASTNode_deepCopy(ASTNode_getLeftChild(ast)));
      tmp3 = ASTNode_createWithType(AST_REAL);
      ASTNode_setReal(tmp3, -0.5);
      ASTNode_addChild(tmp2, tmp3);
      ASTNode_addChild(right, tmp2);
      break;
    case AST_FUNCTION_SIN :
      ASTNode_setType(tmp, AST_TIMES);
      ASTNode_addChild(tmp, differentiate(ASTNode_getLeftChild(ast), target));
      right = ASTNode_createWithType(AST_FUNCTION_COS);
      ASTNode_addChild(right, ASTNode_deepCopy(ASTNode_getLeftChild(ast)));
      ASTNode_addChild(tmp, right);
      break;
    case AST_FUNCTION_COS :
      ASTNode_setType(tmp, AST_TIMES);
      left = ASTNode_createWithType(AST_TIMES);
      tmp2 = ASTNode_createWithType(AST_INTEGER);
      ASTNode_setInteger(tmp2, -1);
      ASTNode_addChild(left, tmp2);
      ASTNode_addChild(left, differentiate(ASTNode_getLeftChild(ast), target));
      ASTNode_addChild(tmp, left);
      right = ASTNode_createWithType(AST_FUNCTION_SIN);
      ASTNode_addChild(right, ASTNode_deepCopy(ASTNode_getLeftChild(ast)));
      ASTNode_addChild(tmp, right);
      break;
    case AST_FUNCTION_TAN :
      ASTNode_setType(tmp, AST_TIMES);
      ASTNode_addChild(tmp, differentiate(ASTNode_getLeftChild(ast), target));
      right = ASTNode_createWithType(AST_PLUS);
      tmp2 = ASTNode_createWithType(AST_INTEGER);
      ASTNode_setInteger(tmp2, 1);
      ASTNode_addChild(right, tmp2);
      tmp2 = ASTNode_createWithType(AST_FUNCTION_POWER);
      tmp3 = ASTNode_createWithType(AST_FUNCTION_TAN);
      ASTNode_addChild(tmp3, ASTNode_deepCopy(ASTNode_getLeftChild(ast)));
      ASTNode_addChild(tmp2, tmp3);
      tmp3 = ASTNode_createWithType(AST_INTEGER);
      ASTNode_setInteger(tmp3, 2);
      ASTNode_addChild(tmp2, tmp3);
      ASTNode_addChild(right, tmp2);
      break;
    case AST_FUNCTION_SINH :
      ASTNode_setType(tmp, AST_TIMES);
      ASTNode_addChild(tmp, differentiate(ASTNode_getLeftChild(ast), target));
      right = ASTNode_createWithType(AST_FUNCTION_COSH);
      ASTNode_addChild(right, ASTNode_deepCopy(ASTNode_getLeftChild(ast)));
      ASTNode_addChild(tmp, right);
      break;
    case AST_FUNCTION_COSH :
      ASTNode_setType(tmp, AST_TIMES);
      ASTNode_addChild(tmp, differentiate(ASTNode_getLeftChild(ast), target));
      right = ASTNode_createWithType(AST_FUNCTION_SINH);
      ASTNode_addChild(right, ASTNode_deepCopy(ASTNode_getLeftChild(ast)));
      ASTNode_addChild(tmp, right);
      break;
    case AST_FUNCTION_TANH :
      ASTNode_setType(tmp, AST_TIMES);
      ASTNode_addChild(tmp, differentiate(ASTNode_getLeftChild(ast), target));
      right = ASTNode_createWithType(AST_DIVIDE);
      tmp2 = ASTNode_createWithType(AST_INTEGER);
      ASTNode_setInteger(tmp2, 1);
      ASTNode_addChild(right, tmp2);
      tmp2 = ASTNode_createWithType(AST_FUNCTION_POWER);
      tmp3 = ASTNode_createWithType(AST_FUNCTION_COSH);
      ASTNode_addChild(tmp3, ASTNode_deepCopy(ASTNode_getLeftChild(ast)));
      ASTNode_addChild(tmp2, tmp3);
      tmp3 = ASTNode_createWithType(AST_INTEGER);
      ASTNode_setInteger(tmp3, 2);
      ASTNode_addChild(tmp2, tmp3);
      ASTNode_addChild(right, tmp2);
      break;
    case AST_REAL :
    case AST_INTEGER :
    case AST_NAME_TIME :
      ASTNode_setType(tmp, AST_INTEGER);
      ASTNode_setInteger(tmp, 0);
      break;
    case AST_NAME :
      ASTNode_setType(tmp, AST_INTEGER);
      if (strcmp(ASTNode_getName(ast), target) == 0) {
        ASTNode_setInteger(tmp, 1);
      } else {
        ASTNode_setInteger(tmp, 0);
      }
      break;
    default :
      printf("unknown\n");
      exit(1);
  }
  return tmp;
}
 */

bool MathUtil::containsTarget(const ASTNode *ast, std::string target)
{
  bool found = false;
  if (ast->getLeftChild() != NULL) {
    found |= containsTarget(ast->getLeftChild(), target);
  }
  if (ast->getRightChild() != NULL) {
    found |= containsTarget(ast->getRightChild(), target);
  }
  if (ast->getType() == AST_NAME) {
    std::string name = ast->getName();
    if (name == target) {
      return true;
    }
  }
  return found;
}

ASTNode* MathUtil::simplify(ASTNode *ast) {
  ASTNodeType_t type = ast->getType();
  ASTNode *left, *right, *tmp;
  long lval, rval;

  if ((!ast->isOperator()) && (type != AST_FUNCTION_POWER)) {
    return ast->deepCopy();
  }

  left  = simplify(ast->getLeftChild());
  right = simplify(ast->getRightChild());

  switch (type) {
    case AST_PLUS :
      if (left->isInteger()) {
        lval = left->getInteger();
        if (lval == 0) {
          return right->deepCopy();
        } else if (right->isInteger()) {
          rval   = right->getInteger();
          tmp = new ASTNode(AST_INTEGER);
          tmp->setValue(lval+rval);
          return tmp;
        }
      }
      if (right->isInteger()) { // left isn't integer
        rval = right->getInteger();
        if (rval == 0) return left->deepCopy();
      }
      break;
    case AST_MINUS :
      if (right->isInteger()) {
        rval = right->getInteger();
        if (rval == 0) {
          return left->deepCopy();
        } else if (left->isInteger()) {
          lval   = left->getInteger();
          tmp = new ASTNode(AST_INTEGER);
          tmp->setValue(lval-rval);
          return tmp;
        }
      }
      break;
    case AST_TIMES :
      if (left->isInteger()) {
        lval = left->getInteger();
        if (lval == 0) {
          tmp = new ASTNode(AST_INTEGER);
          tmp->setValue(0);
          return tmp;
        } else if (lval == 1) {
          return right->deepCopy();
        } else if (right->isInteger()) {
          rval   = right->getInteger();
          tmp = new ASTNode(AST_INTEGER);
          tmp->setValue(lval*rval);
          return tmp;
        }
      }
      if (right->isInteger()) { // left isn't integer
        rval = right->getInteger();
        if (rval == 0) {
          tmp = new ASTNode(AST_INTEGER);
          tmp->setValue(0);
          return tmp;
        } else if (rval == 1) {
          return left->deepCopy();
        }
      }
      break; // can't simplify
    case AST_DIVIDE :
      if (left->isInteger()) {
        lval = left->getInteger();
        if (lval == 0) {
          tmp = new ASTNode(AST_INTEGER);
          tmp->setValue(0);
          return tmp;
        } else if (right->isInteger()) {
          rval   = right->getInteger();
          tmp = new ASTNode(AST_REAL);
          tmp->setValue((double)lval/rval);
          return tmp;
        }
      }
      if (right->isInteger()) { // left isn't integer
        rval = right->getInteger();
        if (rval == 1) {
          return left->deepCopy();
        }
      }
      break; // can't simplify
    default :
      break;
  }
  tmp = new ASTNode(type);
  tmp->addChild(left);
  tmp->addChild(right);
  return tmp;
}
