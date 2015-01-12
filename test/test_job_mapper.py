from imp import load_source
from nose.tools.nontrivial import with_setup
job_mapper = load_source( 'job_mapper', 'condor/job_mapper' )

_globals = {'options_get_mode' : []}

def setup_func():
    "set up test fixtures"
    available_options = ['process', 'cores', 'sample', 'mode', 'cmssw', 'energy',
               "return_sample", "return_mode", "return_cmssw", "return_energy",
               "return_cores",
               ]
    _globals['options_get_mode'] = ['--energy', '7', '--process', '0', '--return_mode'] 

def teardown_func():
    "tear down test fixtures"

@with_setup(setup_func, teardown_func)
def test_get_mode_process_0():
    params, _ = job_mapper.parse_args(_globals['options_get_mode'])
    assert job_mapper.main(params) == 'central'

@with_setup(setup_func, teardown_func)
def test_build_matrix():
    matrix_7TeV = job_mapper.build_matrix(energy = 7)
    matrix_8TeV = job_mapper.build_matrix(energy = 8)
    
    assert len(matrix_7TeV) == (len(job_mapper.samples) - len(job_mapper.ignore_samples_7TeV)) * len(job_mapper.analysis_modes)
    assert len(matrix_8TeV) == (len(job_mapper.samples) - len(job_mapper.ignore_samples_8TeV)) * len(job_mapper.analysis_modes)
    
    # test if all are unique
    set_7TeV = set(matrix_7TeV)
    assert len(set_7TeV) == len(matrix_7TeV)
    
    set_8TeV = set(matrix_8TeV)
    assert len(set_8TeV) == len(matrix_8TeV)
    