. clean.sh
python reader.py --InPath=/lustre/ific.uv.es/grid/atlas/t3/cardillo/ttW-CA/light-Ntup-20-01-21/eq_0z_geq_2j_lt_4j_eq_1b --OutPath=eq_0z_geq_2j_lt_4j_eq_1b/
python reader.py --InPath=/lustre/ific.uv.es/grid/atlas/t3/cardillo/ttW-CA/light-Ntup-20-01-21/eq_0z_geq_2j_lt_4j_eq_2b --OutPath=eq_0z_geq_2j_lt_4j_eq_2b/
python reader.py --InPath=/lustre/ific.uv.es/grid/atlas/t3/cardillo/ttW-CA/light-Ntup-20-01-21/eq_0z_geq_4j_eq_1b --OutPath=eq_0z_geq_4j_eq_1b/
python reader.py --InPath=/lustre/ific.uv.es/grid/atlas/t3/cardillo/ttW-CA/light-Ntup-20-01-21/eq_0z_geq_4j_eq_2b --OutPath=eq_0z_geq_4j_eq_2b/
python reader.py --InPath=/lustre/ific.uv.es/grid/atlas/t3/cardillo/ttW-CA/light-Ntup-20-01-21/eq_1z_geq_2j_eq_2b --OutPath=eq_1z_geq_2j_eq_2b/

python mergeRegions.py --Dir=eq_
