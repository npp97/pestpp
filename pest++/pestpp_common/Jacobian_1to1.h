#ifndef JACOBIAN_1TO1H_
#define JACOBIAN_1TO1H_
#include<map>
#include <unordered_map>
#include <vector>
#include <string>
#include "Transformable.h"
#include "Jacobian.h"

class ParamTransformSeq;
class ParameterInfo;
class ParameterGroupInfo;
class RunManagerAbstract;
class ObjectiveFunc;
class ModelRun;
class FileManager;
class PriorInformation;

class Jacobian_1to1 : public Jacobian{

public:
	Jacobian_1to1(FileManager &_file_manager);
	virtual bool calculate(ModelRun &model_run, vector<string> numeric_par_names, vector<string> obs_names, ParamTransformSeq &par_transform,
		const ParameterGroupInfo &group_info, const ParameterInfo &ctl_par_info, 
		RunManagerAbstract &run_manager, const PriorInformation &prior_info, set<string> &out_of_bound_par, bool phiredswh_flag=false, bool calc_init_obs=true);
	virtual bool calculate(ModelRun &model_run, ParamTransformSeq &par_transform, const ParameterGroupInfo &group_info, const ParameterInfo &ctl_par_info, 
		RunManagerAbstract &run_manager, const PriorInformation &prior_info, set<string> &out_of_bound_par, bool phiredswh_flag=false, bool calc_init_obs=true);
	virtual ~Jacobian_1to1();
protected:
	bool forward_diff(const string &par_name, double derivative_par_value, 
		const ParameterGroupInfo &group_info, const ParameterInfo &ctl_par_info, const ParamTransformSeq &par_trans, double &new_par_val);
	bool central_diff(const string &par_name, double derivative_par_value, 
		const ParameterGroupInfo &group_info, const ParameterInfo &ctl_par_info, const ParamTransformSeq &par_trans, vector<double> &new_par_vec, 
		vector<Parameters>  &numeric_dir_par_vec);
	bool out_of_bounds(const Parameters &model_parameters, const ParameterGroupInfo &group_info, const ParameterInfo &ctl_par_info) const;
	bool get_derivative_parameters(const string &par_name, double derivative_par_value, const ParamTransformSeq &par_trans, const ParameterGroupInfo &group_info, const ParameterInfo &ctl_par_info, 
		vector<JacobianRun> &del_numeric_par_vec, bool phiredswh_flag);
};

#endif /* JACOBIAN_1TO1H_ */
