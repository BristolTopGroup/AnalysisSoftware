from imp import load_source
from nose.tools.nontrivial import with_setup
import os
dirname, _ = os.path.split(os.path.abspath(__file__))
analysis_info = load_source( 'analysis_info', dirname + '/../python/analysis_info.py' )
job_mapper = load_source( 'job_mapper', dirname + '/../condor/job_mapper' )

_globals = {'options_get_mode' : []}

def setup_func():
    "set up test fixtures"
    available_options = ['process', 'cores', 'sample', 'mode', 'cmssw', 'energy',
               "return_sample", "return_mode", "return_cmssw", "return_energy",
               "return_cores",
               ]
    _globals['options_get_mode'] = ['--energy', '7', '--process', '0', '--return_mode']
    _globals['options_test_simple'] = ['--test'] 

def teardown_func():
    "tear down test fixtures"

@with_setup(setup_func, teardown_func)
def test_get_mode_process_0():
    params, _ = job_mapper.parse_args(_globals['options_get_mode'])
    assert job_mapper.main(params) == 'central'

@with_setup(setup_func, teardown_func)
def test_build_matrix():
    matrix_7TeV, _ = job_mapper.build_matrix(energy = 7)
    matrix_8TeV, _ = job_mapper.build_matrix(energy = 8)
    assert len(matrix_7TeV) == (len(analysis_info.datasets_7TeV.keys()) * len(job_mapper.analysis_modes))
    assert len(matrix_8TeV) == (len(analysis_info.datasets_8TeV.keys()) * len(job_mapper.analysis_modes))
    
    # test if all are unique
    set_7TeV = set(matrix_7TeV)
    assert len(set_7TeV) == len(matrix_7TeV)
    
    set_8TeV = set(matrix_8TeV)
    assert len(set_8TeV) == len(matrix_8TeV)
    
def test_test_option():
    params, _ = job_mapper.parse_args(['--operation', 'test', '--return_mode'])
    assert job_mapper.main(params) == 'central'
    params, _ = job_mapper.parse_args(['--operation', 'test', '--return_sample'])
    assert job_mapper.main(params) == 'TTJet'
    params, _ = job_mapper.parse_args(['--operation', 'test', '--return_energy'])
    assert job_mapper.main(params) == 7
    params, _ = job_mapper.parse_args(['--operation', 'test', '--return_cores'])
    assert job_mapper.main(params) == 1
    params, _ = job_mapper.parse_args(['--operation', 'test', '--return_n_jobs'])
    assert job_mapper.main(params) == 1
    
def test_test_option_with_defaults():
    params, _ = job_mapper.parse_args(['--operation', 'test', '--return_mode', '--mode', 'JES_up'])
    assert job_mapper.main(params) == 'JES_up'
    params, _ = job_mapper.parse_args(['--operation', 'test', '--return_sample', '--sample', 'SingleElectron', '--energy', '8'])
    assert job_mapper.main(params) == 'SingleElectron'
    params, _ = job_mapper.parse_args(['--operation', 'test', '--return_energy', '--energy', '8'])
    assert job_mapper.main(params) == 8
    params, _ = job_mapper.parse_args(['--operation', 'test', '--return_cores', '--cores', '2'])
    assert job_mapper.main(params) == 2
    
def test_noop_option():
    params, _ = job_mapper.parse_args(['--return_noop', '--noop'])
    assert job_mapper.main(params) == 1
    params, _ = job_mapper.parse_args(['--return_noop'])
    assert job_mapper.main(params) == 0
    print 'test'
    
def test_single_option():
    matrix_7TeV, _ = job_mapper.build_matrix(energy = 7, chosen_sample = 'TTJet')
    assert len(matrix_7TeV) == len(job_mapper.analysis_modes)
    