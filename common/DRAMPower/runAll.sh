#!/bin/bash
rm evaluation_out/*
python energy_evaluation_darknet_default_DDR4.py
python energy_evaluation_darknet_default_LPDDR4.py

python energy_evaluation_tvm_default_DDR4.py
python energy_evaluation_tvm_default_LPDDR4.py

#python energy_evaluation_openvino_default_DDR4.py
#python energy_evaluation_openvino_default_LPDDR4.py