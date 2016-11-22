#include "sbmlsim/internal/wrapper/FunctionDefinitionWrapper.h"

FunctionDefinitionWrapper::FunctionDefinitionWrapper(const FunctionDefinition *functionDefinition) {
  this->name = functionDefinition->getName();
  this->body = functionDefinition->getBody()->deepCopy();

  for (auto i = 0; i < functionDefinition->getNumArguments(); i++) {
    auto argument = functionDefinition->getArgument(i);
    auto type = argument->getType();
    switch (type) {
      case AST_NAME:
        this->arguments.push_back(argument->getName());
        break;
      default:
        std::cout << "type = " << type << std::endl;
        break;
    }
  }
}

FunctionDefinitionWrapper::FunctionDefinitionWrapper(const FunctionDefinitionWrapper &functionDefinition) {
  this->name = functionDefinition.name;
  this->arguments = functionDefinition.arguments;
  this->body = functionDefinition.body->deepCopy();
}

FunctionDefinitionWrapper::~FunctionDefinitionWrapper() {
  this->arguments.clear();
  delete this->body;
}

const std::string &FunctionDefinitionWrapper::getName() const {
  return this->name;
}

const std::vector<std::string> &FunctionDefinitionWrapper::getArguments() const {
  return this->arguments;
}

const ASTNode *FunctionDefinitionWrapper::getBody() const {
  return this->body;
}
