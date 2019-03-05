## make directory
if [ ! -d "outMultivar" ]; then
  mkdir outMultivar
fi

## compile
root -l -b <<EOF
.L rootAna_multivar.C++
.q
EOF

### files at: /sphenix/user/lajoie/sPHENIX/jet_simulations/
#for ihcalType in "SS310_OUTPUT"
#for ihcalType in "SS310_OUTPUT" "AL_OUTPUT" "AL_NOIHCAL_OUTPUT" "SS310_FRAME_OUTPUT"
for ihcalType in "SS310_RSCALE_OUTPUT" "AL_RSCALE_OUTPUT" "AL_NOIHCAL_RSCALE_OUTPUT" "SS310_FRAME_RSCALE3_OUTPUT" "SS310_NOIHCAL_RSCALE2_OUTPUT"
do
  echo " ***** RUN rootAna_multivar(ihcalType = "$ihcalType") *****"
  root -l -b -q 'rootAna_multivar.C("'$ihcalType'")'
  echo ""
done
