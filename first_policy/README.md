#First policy

The policies are organized by using ***table*** data structure in Lua.

##Policy Format

```
Entry{
  ip = "187.73.32.0",
  prefix_len = 27,
  group = 1,
  --you can customize the parameters here:
  --[[
      ["params"] = {
        limit = "100 kbps",
        expire = "5 min",
        action = "ACCEPT",
      },
  --]]
}
```

Notice that, the ***Entry*** keyword actually represents a function, which takes a table as input, and can do some useful work, such as statistics, verification, etc. In addition, a ***group*** can be defined as follows:

```
group1 = {                                                                      
  [TCP] = {                                                                 
    [PKT_IN] = {                                                      
      [11371]   = true,                                                   
    }, 
    [PKT_OUT] = {                                                      
      [11371]   = true,                                                   
    },                                                                      
  },                                                                          
  
  [UDP] = {                                                                 
    [PKT_IN] = {                                                      
      [110]   = true,                                                    
    }, 
    [PKT_OUT] = {                                                      
      [110]   = true,                                                  
    },                                                                    
  },
  
  ["params"] = {                                                              
     limit = "15 kbps",                                                      
     expire = "80 sec",                                                      
     action = "ACCEPT",                                                      
  },                                                                          
}
```
