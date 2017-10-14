#foreach prefix (MyTree_Al_NotScaled)
foreach prefix (MyTree_Al_NotScaled MyTree_Al_Scaled_v3)
  echo "***** Run" $prefix
  root -l -b -q rootAna_emfrac.C\(\"$prefix\"\)
end
