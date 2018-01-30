import matplotlib.pyplot as plt

TcpBic1 = 0.865829
TcpBic4 = 0.870537
TcpBic8 = 0.691501
TcpBic16 = 0.739966
TcpBic32 = 0.715882

TcpHighSpeed1 = 0.865829
TcpHighSpeed4 = 0.870537
TcpHighSpeed8 = 0.768593
TcpHighSpeed16 = 0.78985
TcpHighSpeed32 = 0.789492

TcpHtcp1 = 0.865829
TcpHtcp4 = 0.870537
TcpHtcp8 = 0.793753
TcpHtcp16 = 0.792719
TcpHtcp32 = 0.753063

TcpHybla1 = 0.868352
TcpHybla4 = 0.871356
TcpHybla8 = 0.77219
TcpHybla16 = 0.754116
TcpHybla32 = 0.692798

TcpIllinois1 = 0.865829
TcpIllinois4 = 0.870537
TcpIllinois8 = 0.799169
TcpIllinois16 = 0.803142
TcpIllinois32 = 0.798283

TcpNewReno1 = 0.865829
TcpNewReno4 = 0.870537
TcpNewReno8 = 0.800222
TcpNewReno16 = 0.803571
TcpNewReno32 = 0.802785

TcpScalable1 = 0.865829
TcpScalable4 = 0.870537
TcpScalable8 = 0.73591
TcpScalable16 = 0.737322
TcpScalable32 = 0.733748

TcpVegas1 = 0.377844
TcpVegas4 = 0.873466
TcpVegas8 = 0.875252
TcpVegas16 = 0.87611
TcpVegas32 = 0.87611

TcpVeno1 = 0.865829
TcpVeno4 = 0.870537
TcpVeno8 = 0.800222
TcpVeno16 = 0.803571
TcpVeno32 = 0.802785

TcpWestwood1 = 0.865829
TcpWestwood4 = 0.870537
TcpWestwood8 = 0.835149
TcpWestwood16 = 0.844736
TcpWestwood32 = 0.841091

TcpWestwoodPlus1 = 0.865829
TcpWestwoodPlus4 = 0.870537
TcpWestwoodPlus8 = 0.835077
TcpWestwoodPlus16 = 0.844736
TcpWestwoodPlus32 = 0.841091

TcpYeah1 = 0.869321
TcpYeah4 = 0.873466
TcpYeah8 = 0.875252
TcpYeah16 = 0.854884
TcpYeah32 = 0.780416

x=[1,2,3,4,5,6,7,8,9,10,11,12]
myx=['TcpBic','TcpHighSpeed','TcpHtcp','TcpHybla','TcpIllinois','TcpNewReno','TcpScalable','TcpVegas','TcpVeno','TcpWestwood','TcpWestwoodPlus','TcpYeah']
y1=[TcpBic1,TcpHighSpeed1,TcpHtcp1,TcpHybla1,TcpIllinois1,TcpNewReno1,TcpScalable1,TcpVegas1,TcpVeno1,TcpWestwood1,TcpWestwoodPlus1,TcpYeah1]
y4=[TcpBic4,TcpHighSpeed4,TcpHtcp4,TcpHybla4,TcpIllinois4,TcpNewReno4,TcpScalable4,TcpVegas4,TcpVeno4,TcpWestwood4,TcpWestwoodPlus4,TcpYeah4]
y8=[TcpBic8,TcpHighSpeed8,TcpHtcp8,TcpHybla8,TcpIllinois8,TcpNewReno8,TcpScalable8,TcpVegas8,TcpVeno8,TcpWestwood8,TcpWestwoodPlus8,TcpYeah8]
y16=[TcpBic16,TcpHighSpeed16,TcpHtcp16,TcpHybla16,TcpIllinois16,TcpNewReno16,TcpScalable16,TcpVegas16,TcpVeno16,TcpWestwood16,TcpWestwoodPlus16,TcpYeah16]
y32=[TcpBic32,TcpHighSpeed32,TcpHtcp32,TcpHybla32,TcpIllinois32,TcpNewReno32,TcpScalable32,TcpVegas32,TcpVeno32,TcpWestwood32,TcpWestwoodPlus32,TcpYeah32]

plt.plot(x,y1, label='nSpoke=1')
plt.plot(x,y4, label='nSpoke=4')
plt.plot(x,y8, label='nSpoke=8')
plt.plot(x,y16, label='nSpoke=16')
plt.plot(x,y32, label='nSpoke=32')

plt.xticks(x,myx);
plt.xlabel('Tcp Protocol')
plt.ylabel('Tcp Protocol Goodput / Mbps')
plt.title('Total Throughput for TCP Variant verus Number of Spoke')
plt.legend()
plt.show()
