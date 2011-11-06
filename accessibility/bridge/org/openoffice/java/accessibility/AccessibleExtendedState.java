/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



package org.openoffice.java.accessibility;

import javax.accessibility.AccessibleState;

public class AccessibleExtendedState extends AccessibleState {
    public static final AccessibleState DEFUNCT;
    public static final AccessibleState INDETERMINATE;
    public static final AccessibleState MANAGES_DESCENDANTS;
    public static final AccessibleState SENSITIVE;
    public static final AccessibleState STALE;
    
    static {
        DEFUNCT = new AccessibleExtendedState("defunct");
        
        // JAVA 1.5: will come with manages_descendants and indeterminate
        INDETERMINATE = new AccessibleExtendedState("indeterminate");
        MANAGES_DESCENDANTS = new AccessibleExtendedState("managesDescendants");

        SENSITIVE = new AccessibleExtendedState("sensitive");
        STALE = new AccessibleExtendedState("stale");
    }
    
    protected AccessibleExtendedState(String key) {
        super(key);
    }
}
