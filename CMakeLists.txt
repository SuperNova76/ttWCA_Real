# Auto-generated on: 2017-11-24 15:14:15.262411

# Declare the name of this package:
atlas_subdir( ttWCA None )

# This package depends on other packages:
atlas_depends_on_subdirs( PUBLIC
                          TopObjectSelectionTools
                          TopEventSelectionTools
                          TopEvent
                          TopConfiguration
                          TopAnalysis
                          JetAnalysisInterfaces
                          AsgTools )

# This package uses ROOT:
find_package( ROOT REQUIRED COMPONENTS Core Gpad Tree Hist RIO MathCore Graf )

# Generate a CINT dictionary source file:
atlas_add_root_dictionary( ttWCA _cintDictSource
                           ROOT_HEADERS Root/LinkDef.h
                           EXTERNAL_PACKAGES ROOT )

# Build a library that other components can link against:
atlas_add_library( ttWCA Root/*.cxx Root/*.h Root/*.icc
                   ttWCA/*.h ttWCA/*.icc ttWCA/*/*.h
                   ttWCA/*/*.icc ${_cintDictSource} 
                   PUBLIC_HEADERS ttWCA
                   LINK_LIBRARIES TopObjectSelectionTools
                                  TopEventSelectionTools
                                  TopEvent
                                  TopConfiguration
                                  TopAnalysis
                                  JetAnalysisInterfacesLib
                                  AsgTools
                                  ${ROOT_LIBRARIES}
                   INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} )

# Install data files from the package:
atlas_install_data( DIRECTORY share/* )

