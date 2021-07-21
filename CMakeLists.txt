# Declare the name of this package:
atlas_subdir( ttWCA )

# This package depends on other packages:
atlas_depends_on_subdirs( PUBLIC
                          TopObjectSelectionTools
                          TopEventSelectionTools
                          TopEvent
                          TopConfiguration
                          TopAnalysis
                          JetAnalysisInterfaces
                          AsgTools
			  TruthClassification 
                          GammaORTools )

# This package uses ROOT:
find_package( ROOT REQUIRED COMPONENTS Core Gpad Tree Hist RIO MathCore Graf )

# Generate a CINT dictionary source file:
atlas_add_root_dictionary( ttWCA ttWCACintDict
                           ROOT_HEADERS Root/LinkDef.h
                           EXTERNAL_PACKAGES ROOT )

# Build a library that other components can link against:
atlas_add_library( ttWCA
                   ttWCA/*.h Root/*.cxx ${ttWCACintDict}
                   PUBLIC_HEADERS ttWCA
                   LINK_LIBRARIES TopObjectSelectionTools
                                  TopEventSelectionTools
                                  TopEvent
                                  TopConfiguration
                                  TopAnalysis
                                  JetAnalysisInterfacesLib
                                  AsgTools
				  TruthClassificationLib
                                  GammaORToolsLib
                                  ${ROOT_LIBRARIES}
                   INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} )

# Install data files from the package:
atlas_install_data( DIRECTORY share/* )

atlas_add_executable( runMMTool
		      utils/runMMTool.cxx
		      LINK_LIBRARIES ${ROOT_LIBRARIES} ttWCA )
