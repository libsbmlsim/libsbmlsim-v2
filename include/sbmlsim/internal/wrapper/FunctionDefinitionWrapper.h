#ifndef INCLUDE_SBMLSIM_INTERNAL_WRAPPER_FUNCTIONDEFINITIONWRAPPER_H_
#define INCLUDE_SBMLSIM_INTERNAL_WRAPPER_FUNCTIONDEFINITIONWRAPPER_H_

#include <sbml/SBMLTypes.h>
#include <string>
#include <vector>

class FunctionDefinitionWrapper {
public:
  explicit FunctionDefinitionWrapper(const FunctionDefinition *functionDefinition);
  FunctionDefinitionWrapper(const FunctionDefinitionWrapper &functionDefinition);
  ~FunctionDefinitionWrapper();
  const std::string &getName() const;
  const std::vector<std::string> &getArguments() const;
  const ASTNode *getBody() const;
private:
  std::string name;
  std::vector<std::string> arguments;
  const ASTNode *body;
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_WRAPPER_FUNCTIONDEFINITIONWRAPPER_H_ */
