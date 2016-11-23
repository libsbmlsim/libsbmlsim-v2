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

ASTNode* MathUtil::differentiate(const ASTNode *ast, std::string target) {
  ASTNodeType_t type = ast->getType();
  ASTNode *tmp = new ASTNode();
  ASTNode *tmp2, *tmp3;
  ASTNode *left, *right;
  ASTNode *ll, *lr;

  if (containsTarget(ast, target) == 0) {
    tmp->setType(AST_INTEGER);
    tmp->setValue(0);
    return tmp;
  }

  switch (type) {
    case AST_PLUS:
      tmp->setType(AST_PLUS);
      tmp->addChild(differentiate(ast->getLeftChild(), target));
      tmp->addChild(differentiate(ast->getRightChild(), target));
      break;
    case AST_MINUS:
      tmp->setType(AST_MINUS);
      tmp->addChild(differentiate(ast->getLeftChild(), target));
      tmp->addChild(differentiate(ast->getRightChild(), target));
      break;
    case AST_TIMES:
      tmp->setType(AST_PLUS);
      left = new ASTNode(AST_TIMES);
      left->addChild(differentiate(ast->getLeftChild(), target));
      left->addChild(ast->getRightChild()->deepCopy());
      tmp->addChild(left);
      right = new ASTNode(AST_TIMES);
      right->addChild(ast->getLeftChild()->deepCopy());
      right->addChild(differentiate(ast->getRightChild(), target));
      tmp->addChild(right);
      break;
    case AST_DIVIDE:
      tmp->setType(AST_DIVIDE);
      if (containsTarget(ast->getRightChild(), target) == 0) {
        tmp->addChild(differentiate(ast->getLeftChild(),target));
        tmp->addChild(ast->getRightChild()->deepCopy());
        break;
      }
      left = new ASTNode(AST_MINUS);
      ll = new ASTNode(AST_TIMES);
      lr = new ASTNode(AST_TIMES);
      ll->addChild(differentiate(ast->getLeftChild(), target));
      ll->addChild(ast->getRightChild()->deepCopy());
      lr->addChild(ast->getLeftChild()->deepCopy());
      lr->addChild(differentiate(ast->getRightChild(), target));
      left->addChild(ll);
      left->addChild(lr);
      tmp->addChild(left);
      right = new ASTNode(AST_FUNCTION_POWER);
      right->addChild(ast->getRightChild()->deepCopy());
      tmp2 = new ASTNode(AST_INTEGER);
      tmp2->setValue(2);
      right->addChild(tmp2);
      tmp->addChild(right);
      break;
    case AST_FUNCTION_POWER:
    case AST_POWER:
      tmp->setType(AST_TIMES);
      left = new ASTNode(AST_TIMES);
      left->addChild(ast->getRightChild()->deepCopy());
      left->addChild(differentiate(ast->getLeftChild(), target));
      tmp->addChild(left);
      tmp3 = new ASTNode(AST_FUNCTION_POWER);
      tmp3->addChild(ast->getLeftChild()->deepCopy());
      right = new ASTNode(AST_MINUS);
      right->addChild(ast->getRightChild()->deepCopy());
      tmp2 = new ASTNode(AST_INTEGER);
      tmp2->setValue(1);
      right->addChild(tmp2);
      tmp3->addChild(right);
      tmp->addChild(tmp3);
      break;
    case AST_FUNCTION_ROOT:
      tmp->setType(AST_TIMES);
      tmp->addChild(differentiate(ast->getLeftChild(), target));
      right = new ASTNode(AST_TIMES);
      tmp2 = new ASTNode(AST_REAL);
      tmp2->setValue(0.5);
      right->addChild(tmp2);
      tmp2 = new ASTNode(AST_FUNCTION_POWER);
      tmp2->addChild(ast->getLeftChild()->deepCopy());
      tmp3 = new ASTNode(AST_REAL);
      tmp3->setValue(-0.5);
      tmp2->addChild(tmp3);
      right->addChild(tmp2);
      break;
    case AST_FUNCTION_SIN:
      tmp->setType(AST_TIMES);
      tmp->addChild(differentiate(ast->getLeftChild(), target));
      right = new ASTNode(AST_FUNCTION_COS);
      right->addChild(ast->getLeftChild()->deepCopy());
      tmp->addChild(right);
      break;
    case AST_FUNCTION_COS:
      tmp->setType(AST_TIMES);
      left = new ASTNode(AST_TIMES);
      tmp2 = new ASTNode(AST_INTEGER);
      tmp2->setValue(-1);
      left->addChild(tmp2);
      left->addChild(differentiate(ast->getLeftChild(), target));
      tmp->addChild(left);
      right = new ASTNode(AST_FUNCTION_SIN);
      right->addChild(ast->getLeftChild()->deepCopy());
      tmp->addChild(right);
      break;
    case AST_FUNCTION_TAN:
      tmp->setType(AST_TIMES);
      tmp->addChild(differentiate(ast->getLeftChild(), target));
      right = new ASTNode(AST_PLUS);
      tmp2 = new ASTNode(AST_INTEGER);
      tmp2->setValue(1);
      right->addChild(tmp2);
      tmp2 = new ASTNode(AST_FUNCTION_POWER);
      tmp3 = new ASTNode(AST_FUNCTION_TAN);
      tmp3->addChild(ast->getLeftChild()->deepCopy());
      tmp2->addChild(tmp3);
      tmp3 = new ASTNode(AST_INTEGER);
      tmp3->setValue(2);
      tmp2->addChild(tmp3);
      right->addChild(tmp2);
      break;
    case AST_FUNCTION_SINH:
      tmp->setType(AST_TIMES);
      tmp->addChild(differentiate(ast->getLeftChild(), target));
      right = new ASTNode(AST_FUNCTION_COSH);
      right->addChild(ast->getLeftChild()->deepCopy());
      tmp->addChild(right);
      break;
    case AST_FUNCTION_COSH:
      tmp->setType(AST_TIMES);
      tmp->addChild(differentiate(ast->getLeftChild(), target));
      right = new ASTNode(AST_FUNCTION_SINH);
      right->addChild(ast->getLeftChild()->deepCopy());
      tmp->addChild(right);
      break;
    case AST_FUNCTION_TANH:
      tmp->setType(AST_TIMES);
      tmp->addChild(differentiate(ast->getLeftChild(), target));
      right = new ASTNode(AST_DIVIDE);
      tmp2 = new ASTNode(AST_INTEGER);
      tmp2->setValue(1);
      right->addChild(tmp2);
      tmp2 = new ASTNode(AST_FUNCTION_POWER);
      tmp3 = new ASTNode(AST_FUNCTION_COSH);
      tmp3->addChild(ast->getLeftChild()->deepCopy());
      tmp2->addChild(tmp3);
      tmp3 = new ASTNode(AST_INTEGER);
      tmp3->setValue(2);
      tmp2->addChild(tmp3);
      right->addChild(tmp2);
      break;
    case AST_REAL:
    case AST_INTEGER:
    case AST_NAME_TIME:
      tmp->setType(AST_INTEGER);
      tmp->setValue(0);
      break;
    case AST_NAME:
      tmp->setType(AST_INTEGER);
      if (ast->getName() == target) {
        tmp->setValue(1);
      } else {
        tmp->setValue(0);
      }
      break;
    default:
      printf("unknown\n");
      exit(1);
  }
  return tmp;
}

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

ASTNode* MathUtil::simplify(const ASTNode *ast) {
  ASTNodeType_t type = ast->getType();
  ASTNode *left, *right, *tmp;
  long left_val, right_val;

  if ((!ast->isOperator()) && (type != AST_FUNCTION_POWER)) {
    return ast->deepCopy();
  }

  left  = simplify(ast->getLeftChild());
  right = simplify(ast->getRightChild());

  switch (type) {
    case AST_PLUS:
      if (left->isInteger()) {
        left_val = left->getInteger();
        if (left_val == 0) {
          return right->deepCopy();
        } else if (right->isInteger()) {
          right_val   = right->getInteger();
          tmp = new ASTNode(AST_INTEGER);
          tmp->setValue(left_val+right_val);
          return tmp;
        } else {  // left is an integer, right is not an integer (3 + x)
          tmp = new ASTNode(AST_PLUS);
          tmp->addChild(right->deepCopy());
          tmp->addChild(left->deepCopy());
          return tmp;
        }
      }
      if (right->isInteger()) { // left isn't integer
        right_val = right->getInteger();
        if (right_val == 0) {
          return left->deepCopy();
        }
      }
      break;
    case AST_MINUS:
      if (right->isInteger()) {
        right_val = right->getInteger();
        if (right_val == 0) {
          return left->deepCopy();
        } else if (left->isInteger()) {
          left_val   = left->getInteger();
          tmp = new ASTNode(AST_INTEGER);
          tmp->setValue(left_val-right_val);
          return tmp;
        }
      }
      break;
    case AST_TIMES:
      if (left->isInteger()) {
        left_val = left->getInteger();
        if (left_val == 0) {
          tmp = new ASTNode(AST_INTEGER);
          tmp->setValue(0);
          return tmp;
        } else if (left_val == 1) {
          return right->deepCopy();
        } else if (right->isInteger()) {
          right_val   = right->getInteger();
          tmp = new ASTNode(AST_INTEGER);
          tmp->setValue(left_val*right_val);
          return tmp;
        }
      }
      if (right->isInteger()) { // left isn't integer
        right_val = right->getInteger();
        if (right_val == 0) {
          tmp = new ASTNode(AST_INTEGER);
          tmp->setValue(0);
          return tmp;
        } else if (right_val == 1) {
          return left->deepCopy();
        } else {   // left is not an integer, right is an integer. (x * 2)
          tmp = new ASTNode(AST_TIMES);
          tmp->addChild(right->deepCopy());
          tmp->addChild(left->deepCopy());
          return tmp;
        }
      }
      break; // can't simplify
    case AST_DIVIDE:
      if (left->isInteger()) {
        left_val = left->getInteger();
        if (left_val == 0) {
          tmp = new ASTNode(AST_INTEGER);
          tmp->setValue(0);
          return tmp;
        } else if (right->isInteger()) {
          right_val   = right->getInteger();
          tmp = new ASTNode(AST_REAL);
          tmp->setValue((double)left_val/right_val);
          return tmp;
        }
      }
      if (right->isInteger()) { // left isn't integer
        right_val = right->getInteger();
        if (right_val == 1) {
          return left->deepCopy();
        }
      }
      break; // can't simplify
    case AST_POWER:
    case AST_FUNCTION_POWER:
      if (right->isInteger()) {
        right_val = right->getInteger();
        if (right_val == 0) {
          tmp = new ASTNode(AST_INTEGER);
          tmp->setValue(1);
          return tmp;
        } else if (right_val == 1) {
          return left->deepCopy();
        }
      }
      if (type == AST_FUNCTION_POWER) { // convert pow(x, y) to x ^ y
        tmp = new ASTNode(AST_POWER);
        tmp->addChild(left->deepCopy());
        tmp->addChild(right->deepCopy());
        return tmp;
      }
      break; // can't simplify
    default:
      break;
  }
  tmp = new ASTNode(type);
  tmp->addChild(left);
  tmp->addChild(right);
  return tmp;
}
