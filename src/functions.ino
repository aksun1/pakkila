long int timeLoop (long int startMillis, long int interval){ // the delay function

    while(millis() - startMillis < interval){} 
    return millis() - startMillis;
}

long runningAverage(int M,int dimmer) {
  #define LM_SIZE 10
  static int LM[2][LM_SIZE];      // LastMeasurements
  static byte index[2] = {0,0};
  static long sum[2] = {0,0};
  static byte count[2] = {0,0};

  // keep sum updated to improve speed.
  sum[dimmer] -= LM[dimmer][index[dimmer]];
  LM[dimmer][index[dimmer]] = M;
  sum[dimmer] += LM[dimmer][index[dimmer]];
  index[dimmer]++;
  index[dimmer] = index[dimmer] % LM_SIZE;
  if (count[dimmer] < LM_SIZE) count[dimmer]++;

  return sum[dimmer] / count[dimmer];
}