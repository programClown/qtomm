target_sources(libqtomm PRIVATE
  propertyconfigwidget.cpp
  propertyconfigwidget.h
  propertywidget.cpp
  propertywidget.h
  abstractedit.h
  multivalueedit.h
)

add_subdirectory(boolpropertywidget)
add_subdirectory(colorpropertywidget)
add_subdirectory(numericpropertywidget)
add_subdirectory(floatpropertywidget)
add_subdirectory(floatvectorpropertywidget)
add_subdirectory(integerpropertywidget)
add_subdirectory(integervectorpropertywidget)
add_subdirectory(optionpropertywidget)
add_subdirectory(referencepropertywidget)
add_subdirectory(stringpropertywidget)
add_subdirectory(splinepropertywidget)
add_subdirectory(triggerpropertywidget)
add_subdirectory(vectorpropertywidget)
