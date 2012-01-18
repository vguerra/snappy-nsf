# -*- Tcl -*-
# API declarations for snappy library
#

# namespaces for types of methods
array set ns {
  cmd  "::snappy"
}

# not needed at the moment
#array set ptrConverter {
#  snappy 1
#}


cmd compress NsfsnappyCompress {
  {-argName "input" -required 1 -type tclobj}
}

cmd decompress NsfsnappyDecompress {
  {-argName "input" -required 1 -type tclobj}
}

cmd validate NsfsnappyValidate {
  {-argName "input" -required 1 -type tclobj}
}
