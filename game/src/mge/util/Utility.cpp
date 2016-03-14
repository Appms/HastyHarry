#include "mge/util/Utility.hpp"
#include <string>
#include <vector>

std::vector<std::string> Utility::Split(std::string str, char delemiter)
{
	std::vector<std::string> ret;

	size_t pos = str.find_first_of(delemiter);

	while (!str.empty())
	{
		std::string cur = str.substr(0, pos);
		if (!cur.empty())
			ret.push_back(cur);

		if (pos == std::string::npos)
			break;

		str = str.substr(pos + 1);

		pos = str.find_first_of(delemiter);
	}

	return ret;
}


neV3 Utility::glmToNe(glm::vec3 v)
{
	neV3 result;
	result.Set(v.x, v.y, v.z);
	return result;
}

glm::vec3 Utility::neToGlm(neV3 v)
{
	return glm::vec3(v[0], v[1], v[2]);
}

glm::vec3 Utility::StrToVec(std::string s1, std::string s2, std::string s3)
{
	return glm::vec3(atof(s1.c_str()), atof(s2.c_str()), atof(s3.c_str()));
}