#ifndef INCLUDE_SBMLSIM_CONFIG_OUTPUTFIELD_H_
#define INCLUDE_SBMLSIM_CONFIG_OUTPUTFIELD_H_

#include <string>
#include <ostream>

enum class OutputType;

class OutputField {
 public:
  OutputField(const std::string &id, const OutputType &type);
  ~OutputField();
  const std::string &getId() const;
  const OutputType &getType() const;
 private:
  const std::string id;
  const OutputType type;
};

enum class OutputType {
  AMOUNT,
  CONCENTRATION
};

std::ostream &operator<<(std::ostream &s, const OutputField &obj);

#endif /* INCLUDE_SBMLSIM_CONFIG_OUTPUTFIELD_H_ */
