#include "RunManagerFortranWrap.h"
#include "utilities.h"
#include "RunManagerYAMR.h"
#include "RunManagerSerial.h"
#include "RunManagerGenie.h"
#include "pest_error.h"

typedef struct RunManagerAbstract RunManagerAbstract;

static RunManagerAbstract *_run_manager_ptr_;
static ofstream fout_run_manager_log_file;

using namespace pest_utils;

extern "C"
{

void RMIF_CREATE_SERIAL(char *f_comline, int  *comline_str_len, int *comline_array_len,
					 char *f_tpl, int  *tpl_str_len, int *tpl_array_len,
					 char *f_inp, int  *inp_str_len, int *inp_array_len,
					 char *f_ins, int  *ins_str_len, int *ins_array_len,
					 char *f_out, int  *out_str_len, int *out_array_len,
					 char *f_storfile, int *storfile_len,
					 char *f_rundir, int *rundir_len)
{
	vector<string> comline_vec =  fortran_str_array_2_vec(f_comline, *comline_str_len, *comline_array_len);
	vector<string> tpl_vec =  fortran_str_array_2_vec(f_tpl, *tpl_str_len, *tpl_array_len);
	vector<string> inp_vec =  fortran_str_array_2_vec(f_inp, *inp_str_len, *inp_array_len);
	vector<string> ins_vec =  fortran_str_array_2_vec(f_ins, *ins_str_len, *ins_array_len);
	vector<string> out_vec =  fortran_str_array_2_vec(f_out, *out_str_len, *out_array_len);
	string storfile =  fortran_str_2_string(f_storfile, *storfile_len);
	string rundir =  fortran_str_2_string(f_rundir, *rundir_len);
	_run_manager_ptr_ = new RunManagerSerial(comline_vec, tpl_vec, inp_vec, ins_vec, out_vec, storfile, rundir);

}


void RMIF_CREATE_YAMR(char *f_comline, int  *comline_str_len, int *comline_array_len,
					 char *f_tpl, int  *tpl_str_len, int *tpl_array_len,
					 char *f_inp, int  *inp_str_len, int *inp_array_len,
					 char *f_ins, int  *ins_str_len, int *ins_array_len,
					 char *f_out, int  *out_str_len, int *out_array_len,
					 char *f_storfile, int *storfile_len,
					 char *f_port, int *f_port_len, 
					 char *f_info_filename, int *info_filename_len )
{
	vector<string> comline_vec =  fortran_str_array_2_vec(f_comline, *comline_str_len, *comline_array_len);
	vector<string> tpl_vec =  fortran_str_array_2_vec(f_tpl, *tpl_str_len, *tpl_array_len);
	vector<string> inp_vec =  fortran_str_array_2_vec(f_inp, *inp_str_len, *inp_array_len);
	vector<string> ins_vec =  fortran_str_array_2_vec(f_ins, *ins_str_len, *ins_array_len);
	vector<string> out_vec =  fortran_str_array_2_vec(f_out, *out_str_len, *out_array_len);
	string storfile =  fortran_str_2_string(f_storfile, *storfile_len);
	string port =  fortran_str_2_string(f_port, *f_port_len);
	string info_filename =  fortran_str_2_string(f_port, *info_filename_len);
	fout_run_manager_log_file.open(info_filename);
	_run_manager_ptr_ = new RunManagerYAMR(comline_vec, tpl_vec, inp_vec, ins_vec, out_vec, storfile, port, fout_run_manager_log_file);
}

void RMIF_CREATE_GENIE(char *f_comline, int  *comline_str_len, int *comline_array_len,
					 char *f_tpl, int  *tpl_str_len, int *tpl_array_len,
					 char *f_inp, int  *inp_str_len, int *inp_array_len,
					 char *f_ins, int  *ins_str_len, int *ins_array_len,
					 char *f_out, int  *out_str_len, int *out_array_len,
					 char *f_storfile, int *storfile_len,
					 char *f_host, int *f_host_len,
					 char *f_genie_tag, int *genie_tag_len)
{

	vector<string> comline_vec =  fortran_str_array_2_vec(f_comline, *comline_str_len, *comline_array_len);
	vector<string> tpl_vec =  fortran_str_array_2_vec(f_tpl, *tpl_str_len, *tpl_array_len);
	vector<string> inp_vec =  fortran_str_array_2_vec(f_inp, *inp_str_len, *inp_array_len);
	vector<string> ins_vec =  fortran_str_array_2_vec(f_ins, *ins_str_len, *ins_array_len);
	vector<string> out_vec =  fortran_str_array_2_vec(f_out, *out_str_len, *out_array_len);
	string storfile =  fortran_str_2_string(f_storfile, *storfile_len);
	string host =  fortran_str_2_string(f_host, *f_host_len);
	string genie_tag =  fortran_str_2_string(f_genie_tag, *genie_tag_len);
	_run_manager_ptr_ = new RunManagerGenie(comline_vec, tpl_vec, inp_vec, ins_vec, out_vec, storfile, genie_tag);
}


int RMIF_ADD_RUN(double *parameter_data, int *npar)
{
	int ret_val = 0;
	vector<double> data(parameter_data, parameter_data+*npar);
	ret_val = _run_manager_ptr_->add_run(data);
	return ret_val;
}

int RMIF_INITIALIZE(char *f_pname, int  *pname_str_len, int *pname_array_len,
				 char *f_oname, int  *oname_str_len, int *oname_array_len)

{
	vector<string> pname_vec =  fortran_str_array_2_vec(f_pname, *pname_str_len, *pname_array_len);
	vector<string> oname_vec =  fortran_str_array_2_vec(f_oname, *oname_str_len, *oname_array_len);
	_run_manager_ptr_->initialize(pname_vec, oname_vec);
	return 0;
}

void RMIF_RUN()
{
	_run_manager_ptr_->run();
}

int RMIF_GET_RUN(int *run_id, double *parameter_data, int *npar, double *obs_data, int *nobs)
{
	int err = 0;
	size_t n_par = *npar;
	size_t n_obs = *nobs;
	try {
	err = _run_manager_ptr_->get_run(*run_id, parameter_data, n_par, obs_data, n_obs);
	}
	catch(PestIndexError ex) {
		cerr << ex.what() << endl;
		err = 1;
	}
	return err;
}

void RMFI_DELETE()
{
	delete _run_manager_ptr_;
}
			 
}