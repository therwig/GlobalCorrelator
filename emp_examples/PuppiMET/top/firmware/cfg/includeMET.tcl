#open_project top/top.xpr

set metRoot ../../src/hlsmet
source ${metRoot}/config.tcl

set metDir ${metRoot}/proj0/solution1/impl/ip/
set metIpName ${l1metAlgo}
set metIpModuleName ${metIpName}_0

set ipRepoDir user_ip_repo
file mkdir $ipRepoDir
set_property  ip_repo_paths  $ipRepoDir [current_project]
# Rebuild user ip_repo's index before adding any source files
update_ip_catalog -rebuild
update_ip_catalog -add_ip "$metDir/cern-cms_hls_${metIpName}_[string map { . _ } ${l1metIPVersion}].zip" -repo_path $ipRepoDir

create_ip -name ${metIpName} -vendor cern-cms -library hls -version ${l1metIPVersion} -module_name ${metIpModuleName}
generate_target {instantiation_template} [get_files top/top.srcs/sources_1/ip/${metIpModuleName}/${metIpModuleName}.xci]
generate_target all [get_files top/top.srcs/sources_1/ip/${metIpModuleName}/${metIpModuleName}.xci]
export_ip_user_files -of_objects [get_files top/top.srcs/sources_1/ip/${metIpModuleName}/${metIpModuleName}.xci] -no_script -force -quiet
create_ip_run [get_files -of_objects [get_fileset sources_1] top/top.srcs/sources_1/ip/${metIpModuleName}/${metIpModuleName}.xci]
launch_run -jobs 8 ${metIpModuleName}_synth_1
