#**************************************************************
#  
#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.
#  
#**************************************************************



$(eval $(call gb_Library_Library,reg))

$(eval $(call gb_Library_add_package_headers,reg,registry_inc))

$(eval $(call gb_Library_add_precompiled_header,reg,$(SRCDIR)/registry/inc/pch/precompiled_registry))

$(eval $(call gb_Library_set_versionmap,reg,$(SRCDIR)/registry/util/reg.map))

$(eval $(call gb_Library_set_include,reg,\
	$$(INCLUDE) \
	-I$(SRCDIR)/registry/inc/pch \
	-I$(OUTDIR)/inc \
))

#$(eval $(call gb_Library_add_api,reg, \
#        udkapi \
#	offapi \
#))

$(eval $(call gb_Library_add_defs,reg,\
        -DREGISTRY_DLLIMPLEMENTATION \
))

$(eval $(call gb_Library_add_linked_libs,reg,\
	sal \
	store \
	stl \
	$(gb_STDLIBS) \
))

$(eval $(call gb_Library_add_exception_objects,reg,\
	registry/source/regimpl \
	registry/source/regkey \
	registry/source/registry \
	registry/source/keyimpl \
	registry/source/reflread \
	registry/source/reflwrit \
))

# vim: set noet sw=4 ts=4:

