#eyJidWlsZFByZXNldHMiOnsiZGVmYXVsdF9jb25maWciOnsibmFtZSI6ImRlZmF1bHRfY29uZmlnIiwiZGVmaW5pdGlvbnMiOltdLCJyZW1vdmVkRmxhZ3MiOnsiQyI6WyItT3MiXSwiQ1hYIjpbXSwiQVNNIjpbXX0sImFkZGl0aW9uYWxGbGFncyI6eyJDIjpbIi1PMCJdLCJDWFgiOltdLCJBU00iOltdfX19LCJkaXJlY3RvcmllcyI6W119
target_sources(federated_learning_BLE_peripheral PRIVATE
	"../advertiser.c"
	"../gatt_handlers.c"
)

get_target_property(interface_compile_options slc_federated_learning_BLE_peripheral INTERFACE_COMPILE_OPTIONS)
	list(REMOVE_ITEM interface_compile_options $<$<AND:$<CONFIG:default_config>,$<COMPILE_LANGUAGE:C>>:-Os>)
set_target_properties(federated_learning_BLE_peripheral PROPERTIES INTERFACE_COMPILE_OPTIONS "${interface_compile_options}")

target_compile_options(federated_learning_BLE_peripheral PRIVATE
	$<$<AND:$<CONFIG:default_config>,$<COMPILE_LANGUAGE:C>>:-O0>
)
