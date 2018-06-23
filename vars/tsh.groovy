def call(String args, String interpreter = 'cmd') {
  if (isUnix) {
    sh args
  } else {
    if (interpreter == 'cmd') {
      cmd args
    } else {
      powershell args
    }
  }
}
