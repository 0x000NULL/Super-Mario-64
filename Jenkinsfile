pipeline {
  agent any
  stages {
    stage('Build Tools') {
      steps {
        sh 'make -C tools/'
      }
    }
    stage('Build J Source') {
      steps {
        sh 'make VERSION=jp'
      }
    }
    stage('Build U Source') {
      steps {
        sh 'make VERSION=us'
      }
    }
    stage('Build J Source, NON_MATCHING') {
      steps {
        sh 'make VERSION=jp NON_MATCHING=1'
      }
    }
    stage('Build U Source, NON_MATCHING') {
      steps {
        sh 'make VERSION=us NON_MATCHING=1'
      }
    }
  }
  environment {
    QEMU_IRIX = '/data/n64dev/irixsys/qemu-irix'
  }
}
