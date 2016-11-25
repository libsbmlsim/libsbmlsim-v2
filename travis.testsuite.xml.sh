#!/bin/sh

CWD=`pwd`

cat <<EOM
<testSuiteSettings>
   <wrappers>
      <wrapperConfig>
         <name>libsbmlsim-v2</name>
         <program>${CWD}/build/test/integration/wrapper.py</program>
         <outputPath>${CWD}/build/test/integration</outputPath>
         <arguments>%d %n %o %l %v</arguments>
         <unsupportedTags>
            <string>comp</string>
            <string>fbc</string>
         </unsupportedTags>
         <viewOnly>false</viewOnly>
         <supportsAllVersions>true</supportsAllVersions>
         <concurrentThreadsOK>false</concurrentThreadsOK>
      </wrapperConfig>
      <wrapperConfig>
         <name>-- no wrapper --</name>
         <program></program>
         <outputPath></outputPath>
         <arguments></arguments>
         <unsupportedTags/>
         <viewOnly>false</viewOnly>
         <supportsAllVersions>false</supportsAllVersions>
         <concurrentThreadsOK>false</concurrentThreadsOK>
      </wrapperConfig>
   </wrappers>
   <casesDir>${CWD}/build/test/integration/testsuite/cases/semantic</casesDir>
   <lastWrapper>libsbmlsim-v2</lastWrapper>
   <lastLVcombo>
      <level>0</level>
      <version>0</version>
      <text>SBML Level 0 Version 0</text>
   </lastLVcombo>
</testSuiteSettings>
EOM
