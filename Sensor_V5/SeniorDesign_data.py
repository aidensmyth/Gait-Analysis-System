import re
from collections import OrderedDict

with open('text.txt') as f:
    lines = f.readlines()

lines = ''.join(lines)

knee = re.findall(r'\bKR: \S.*', lines)
ankle = re.findall(r'\bAR: \S.*', lines)
hip = re.findall(r'\bHR: \S.*', lines)
print(knee)
# Sorting Data knee

knee_x = []
knee_y = []
knee_z = []

for i in range(len(knee)):
    
    j  = re.search(r'\bx:\d*\.\d*|\bx:-\d*\.\d*',knee[i])
    if j != None:
        j = j[0]
    k  = re.search(r'\by:\d*\.\d*|\by:-\d*\.\d*',knee[i])
    if k != None:
        k = k[0]
    l  = re.search(r'\bz:\d*\.\d*|\bz:-\d*\.\d*',knee[i])
    if l != None:
        l = l[0]
    
    knee_x.append(j)
    knee_y.append(k)
    knee_z.append(l)
    
None_filter_knee = []    
if None in knee_x:   
    None_filter1 = [i for i,v in enumerate(knee_x) if v == None]
else:
   None_filter1 = []
if None in knee_y:   
    None_filter2 = [i for i,v in enumerate(knee_y) if v == None]
else:
   None_filter2 = []
if None in knee_z:
    None_filter3 = [i for i,v in enumerate(knee_z) if v == None]
else:
   None_filter3 = []
    
None_filter_knee = None_filter1 + None_filter2 + None_filter3
None_filter_knee = list(OrderedDict.fromkeys(None_filter_knee))    


# Sorting data hip

hip_x = []
hip_y = []
hip_z = []

for i in range(len(hip)):
    j  = re.search(r'\bx:\d*\.\d*|\bx:-\d*\.\d*',hip[i])
    if j != None:
        j = j[0]
    k  = re.search(r'\by:\d*\.\d*|\by:-\d*\.\d*',hip[i])
    if k != None:
        k = k[0]
    l  = re.search(r'\bz:\d*\.\d*|\bz:-\d*\.\d*',hip[i])
    if l != None:
        l = l[0]
    
    hip_x.append(j)
    hip_y.append(k)
    hip_z.append(l)

None_filter_hip = []
if None in hip_x:   
    None_filter1 = [i for i,v in enumerate(hip_x) if v == None]
else:
   None_filter1 = []
if None in hip_y:   
    None_filter2 = [i for i,v in enumerate(hip_y) if v == None]
else:
   None_filter2 = []
if None in hip_z:
    None_filter3 = [i for i,v in enumerate(hip_z) if v == None]
else:
   None_filter3 = []
   
None_filter_hip = None_filter1 + None_filter2 + None_filter3  
None_filter_hip = list(OrderedDict.fromkeys(None_filter_hip))
 
# Sorting data ankle
    
ankle_x = []
ankle_y = []
ankle_z = []

for i in range(len(ankle)):
    
    j  = re.search(r'\bx:\d*\.\d*|\bx:-\d*\.\d*',ankle[i])
    if j != None:
        j = j[0]
    k  = re.search(r'\by:\d*\.\d*|\by:-\d*\.\d*',ankle[i])
    if k != None:
        k = k[0]
    l  = re.search(r'\bz:\d*\.\d*|\bz:-\d*\.\d*',ankle[i])
    if l != None:
        l = l[0]
        
    ankle_x.append(j)
    ankle_y.append(k)
    ankle_z.append(l)
print(ankle_x)
None_filter_ankle = []
if None in ankle_x:   
    None_filter1 = [i for i,v in enumerate(ankle_x) if v == None]
else:
   None_filter1 = []
if None in ankle_y:   
    None_filter2 = [i for i,v in enumerate(ankle_y) if v == None]
else:
   None_filter2 = []
if None in ankle_z:
    None_filter3 = [i for i,v in enumerate(ankle_z) if v == None]
else:
   None_filter3 = []
   
None_filter_ankle = None_filter1 + None_filter2 + None_filter3
None_filter_ankle = list(OrderedDict.fromkeys(None_filter_ankle))

# Some stuff

temp = 0 
for i in range(len(None_filter_knee)):
    
    del knee_x[None_filter_knee[i]-temp]
    del knee_y[None_filter_knee[i]-temp]
    del knee_z[None_filter_knee[i]-temp]
    temp+=1

temp = 0 
for i in range(len(None_filter_hip)):
    
    del hip_x[None_filter_hip[i]-temp]
    del hip_y[None_filter_hip[i]-temp]
    del hip_z[None_filter_hip[i]-temp]
    temp+=1
    
temp = 0 
for i in range(len(None_filter_ankle)):
    
    del ankle_x[None_filter_ankle[i]-temp]
    del ankle_y[None_filter_ankle[i]-temp]
    del ankle_z[None_filter_ankle[i]-temp]
    temp+=1

knee_x = [string.replace("x:","") for string in knee_x if "x:" in string]
knee_x = [float(i) for i in knee_x]

knee_y = [string.replace("y:","") for string in knee_y if "y:" in string]
knee_y = [float(i) for i in knee_y]

knee_z = [string.replace("z:","") for string in knee_z if "z:" in string]
knee_z = [float(i) for i in knee_z]


hip_x = [string.replace("x:","") for string in hip_x if "x:" in string]
hip_x = [float(i) for i in hip_x]

hip_y = [string.replace("y:","") for string in hip_y if "y:" in string]
hip_y = [float(i) for i in hip_y]
 
hip_z = [string.replace("z:","") for string in hip_z if "z:" in string]
hip_z = [float(i) for i in hip_z]


ankle_x = [string.replace("x:","") for string in ankle_x if "x:" in string]
ankle_x = [float(i) for i in ankle_x]

ankle_y = [string.replace("y:","") for string in ankle_y if "y:" in string]
ankle_y = [float(i) for i in ankle_y]

ankle_z = [string.replace("z:","") for string in ankle_z if "z:" in string]
ankle_z = [float(i) for i in ankle_z]

#%%
import matplotlib.pyplot as plt
import numpy as np

x_axis_knee_x = np.arange(0,len(knee_x))
plt.figure()
plt.plot(x_axis_knee_x, knee_x)
plt.ylabel("X Euler Angle")
plt.xlabel("Data Points (n)")
plt.title("Knee X Location Data")

x_axis_knee_y = np.arange(0,len(knee_y))
plt.figure()
plt.plot(x_axis_knee_y, knee_y)
plt.ylabel("Y Euler Angle")
plt.xlabel("Data Points (n)")
plt.title("Knee Y Location Data")

x_axis_knee_z = np.arange(0,len(knee_z))
plt.figure()
plt.plot(x_axis_knee_z, knee_z)
plt.ylabel("Z Euler Angle")
plt.xlabel("Data Points (n)")
plt.title("Knee Z Location Data")


x_axis_hip_x = np.arange(0,len(hip_x))
plt.figure()
plt.plot(x_axis_hip_x, hip_x)
plt.ylabel("X Euler Angle")
plt.xlabel("Data Points (n)")
plt.title("Hip X Location Data")

x_axis_hip_y = np.arange(0,len(hip_y))
plt.figure()
plt.plot(x_axis_hip_y, hip_y)
plt.ylabel("Y Euler Angle")
plt.xlabel("Data Points (n)")
plt.title("Hip Y Location Data")

x_axis_hip_z = np.arange(0,len(hip_z))
plt.figure()
plt.plot(x_axis_hip_z, hip_z)
plt.ylabel("Z Euler Angle")
plt.xlabel("Data Points (n)")
plt.title("Hip Z Location Data")

x_axis_ankle_x = np.arange(0,len(ankle_x))
plt.figure()
plt.plot(x_axis_ankle_x, ankle_x)
plt.ylabel("X Euler Angle")
plt.xlabel("Data Points (n)")
plt.title("Ankle X Location Data")

x_axis_ankle_y = np.arange(0,len(ankle_y))
plt.figure()
plt.plot(x_axis_ankle_y, ankle_y)
plt.ylabel("Y Euler Angle")
plt.xlabel("Data Points (n)")
plt.title("Ankle Y Location Data")

x_axis_ankle_z = np.arange(0,len(ankle_z))
plt.figure()
plt.plot(x_axis_ankle_z, ankle_z)
plt.ylabel("Z Euler Angle")
plt.xlabel("Data Points (n)")
plt.title("Ankle Z Location Data")

