#ifndef INCLUDE_SBMLSIM_DEF_H_
#define INCLUDE_SBMLSIM_DEF_H_

#include <string>
#include <map>

using SpeciesId = std::string;
using SpeciesMap = std::map<SpeciesId, double>;
using ParameterId = std::string;
using ParameterMap = std::map<ParameterId, double>;
using ReactionId = std::string;
using GlobalParameterMap = ParameterMap;
using LocalParameterMap = std::map<ReactionId, ParameterMap>;

#endif /* INCLUDE_SBMLSIM_DEF_H_ */
