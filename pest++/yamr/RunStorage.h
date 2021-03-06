/*  
	� Copyright 2012, David Welter
	
	This file is part of PEST++.
   
	PEST++ is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	PEST++ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with PEST++.  If not, see<http://www.gnu.org/licenses/>.
*/

#ifndef RUN_STORAGE_H_
#define RUN_STORAGE_H_

#include <string>
#include <fstream>
#include <ostream>
#include <vector>
#include <cstdint>
#include <Eigen/Dense>

class Parameters;
class Observations;

class RunStorage {
	// This class stores a sequence of model runs in a single binary file using the following format:
	//     nruns (number of model runs stored in file)                       int_64_t
	//     run_size (number of bytes required to store each model run)       int_64_t
	//     par_name_vec_size (number of bytes required to store parameter names)  int_64_t
	//     obes_name_vec_size (number of bytes required to store observation names)  int_64_t
	//     parameter names (serialized parameter names)                               char*par_name_vec_size
	//     observation names (serialized observation names)                               char*obes_name_vec_size
	//   The following structure is repeated nruns times (ie once for each model run)
	//        run_status (flag indicating status of mode run)                         int_8_t
	//              run_status=0   this run has not yet been completed
	//			    run_status=-100   run was canceled
	//				run_status<0 and >-100  run completed and failed.  THis is the number of times it failed
	//				run_status=1   run and been sucessfully completed
	//       parameter_values  (parameters values for model runs)                     double*number of parameters
	//       observationn_values( observations results produced by the model run)     double*number of observations

public:
	RunStorage(const std::string &_filename);
	void reset(const std::vector<std::string> &par_names, const std::vector<std::string> &obs_names, const std::string &_filename = std::string(""));
	void init_restart(const std::string &_filename);
	virtual int add_run(const std::vector<double> &model_pars);
	int add_run(const Parameters &pars);
	virtual int add_run(const Eigen::VectorXd &model_pars);
	void update_run(int run_id, const Parameters &pars, const Observations &obs);
	void update_run(int run_id, const std::vector<char> serial_data);
	void update_run_failed(int run_id);
	int get_nruns();
	int increment_nruns();
	const std::vector<std::string>& get_par_name_vec()const;
	const std::vector<std::string>& get_obs_name_vec()const;
	int get_run_status(int run_id);
	int get_run(int run_id, Parameters *pars, Observations *obs);
	int get_run(int run_id, double *pars, size_t npars, double *obs, size_t nobs);
	Parameters get_parameters(int run_id);
	std::vector<char> get_serial_pars(int run_id);
	void free_memory();
	~RunStorage();
private:
	std::string filename;
	mutable std::fstream buf_stream;
	std::streamoff beg_run0;
	std::streamoff run_byte_size;
	std::streamoff run_par_byte_size;
	std::streamoff run_data_byte_size;
	std::streamoff p_names_size;
	std::streamoff o_names_size;
	std::vector<std::string> par_names;
	std::vector<std::string> obs_names;
	void check_rec_size(const std::vector<char> &serial_data) const;
	void check_rec_id(int run_id);
	std::int8_t get_run_status_native(int run_id);
	std::streamoff get_stream_pos(int run_id);
};

#endif //RUN_STORAGE_H_
