## make directory
if [ ! -d "outMultivar" ]; then
  mkdir outMultivar
fi

## compile
#root -l -b <<EOF
#.L draw_multivar.C++
#.q
#EOF

#for ihcalType in "SS310_OUTPUT"
#for ihcalType in "SS310_OUTPUT" "AL_OUTPUT" "AL_NOIHCAL_OUTPUT" "SS310_FRAME_OUTPUT"
for ihcalType in "SS310_RSCALE_OUTPUT" "AL_RSCALE_OUTPUT" "AL_NOIHCAL_RSCALE_OUTPUT" "SS310_FRAME_RSCALE3_OUTPUT" "SS310_NOIHCAL_RSCALE2_OUTPUT"
do
  echo " ***** RUN draw_multivar(ihcalType = "$ihcalType") *****"
  root -l -b -q 'draw_multivar.C("'$ihcalType'")'
  echo ""
done
