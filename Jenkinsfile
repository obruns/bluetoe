#!/usr/bin/env groovy
// @Library('github.com/obruns/bluetoe@topic/add-jenkinsfile') _
// https://jenkins.io/doc/pipeline/steps/
// https://jenkins.io/doc/pipeline/steps/xunit/#xunit-publish-xunit-test-result-report

buildMatrix [
 ['compiler': 'g++', 'mode': 'Debug', 'label': 'linux'],
 ['compiler': 'g++', 'mode': 'Release', 'label': 'linux'],
 ['compiler': 'clang++', 'mode': 'Debug', 'label': 'linux'],
 ['compiler': 'clang++', 'mode': 'Release', 'label': 'linux'],
// ['mode': 'Debug', 'label': 'windows']  // script defaults to compiler msvc
]

def buildMatrix(List matrix) {
pipeline {
  options {
    buildDiscarder(logRotator(numToKeepStr: '100')) // 100 - Rotation only for test
    timestamps()
  }

  agent none

  environment {
    BUILD_DIR = "build"
  }

  stages {
    stage ("Parallel: Build and Test") {
      steps {
        script {
          def parameterized_stages = [:]

          def bMatrix = matrix

          for (x in bMatrix) {
            def compiler = x['compiler']
            def config = x['mode']
            def CONFIG = "${config}"

            parameterized_stages["${compiler}/${config}"] =  {

              stage("${compiler}/${config}") {
                node ('c++') {
                  try {
                    stage('Checkout') {
                        script {
                            checkout scm
                        }
                    }
                    stage('Cleanup') {
                        script {
                            sh "rm -rf ${BUILD_DIR}_${CONFIG}"
                        }
                    }
                    stage('Run CMake') {
                      environment {
                        GENERATOR = "Ninja"
                      }
                        dir("${BUILD_DIR}_${CONFIG}") {
                          sh """
                            cmake ${WORKSPACE} \
                                  -G Ninja \
                                  -DBLUETOE_EXCLUDE_SLOW_TESTS=1 \
                                  -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE \
                                  -DCMAKE_BUILD_TYPE=${CONFIG} \
                                  -DCMAKE_CXX_COMPILER=${compiler}
                          """
                        }
                    }
                    stage('Build and Link') {
                      environment {
                        NINJA_STATUS = "[%r %c %o %e (%s/%t)]"
                      }
                        sh """
                          cmake --build ${WORKSPACE}/${BUILD_DIR}_${CONFIG} \
                                --config ${CONFIG} \
                                -- -j 1 -k 100 -v -d explain
                        """
                    }
                    stage('Run Boost Tests') {
                        dir("${BUILD_DIR}_${CONFIG}") {
                            sh "ctest --verbose"
                        }
                    }
                  }
                  // https://stackoverflow.com/questions/48989238/post-equivalent-in-scripted-pipeline
                  finally {  // post { always { } }
                    // https://stackoverflow.com/questions/45133596/how-do-i-publish-boost-unit-tests-with-jenkins-pipeline-and-xunit-plugin
                    // https://stackoverflow.com/questions/16537914/using-jenkins-with-boost-test-unit-tests
                    // https://github.com/jenkinsci/xunit-plugin/tree/master/src/main/java/org/jenkinsci/plugins/xunit/types
                    step([$class: 'XUnitBuilder', tools: [[$class: 'BoostTestJunitHudsonTestType', pattern: "${BUILD_DIR}_${CONFIG}/tests/*.xml"]]])
                    deleteDir()
                  }
                }
              }
            }
          }
          parallel parameterized_stages
        }
      }
    }
  }
}
}
