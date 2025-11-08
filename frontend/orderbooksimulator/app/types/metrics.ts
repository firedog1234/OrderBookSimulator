export type DataStructureName = "Red-Black Tree" | "Skip List";

export interface Metrics {
  // data structure type
  name: DataStructureName;

  // timing metrics (will be expressed in microseconds)
  avgInsertTime: number;
  avgDeleteTime: number;
  avgLookupTime: number;
  minLatency: number;
  maxLatency: number;
  medianLatency: number;
  percentile95: number;
  percentile99: number;

  // throughput
  ordersPerSecond: number;
  totalOrders: number;

  // memory usage
  memoryUsageBytes: number;

  // order counts
  addCount: number;
  cancelCount: number;
  modifyCount: number;

  // latency distribution
  latencies: number[];
}

export interface MetricsResponse {
  rbTree?: Metrics;
  skipList?: Metrics;
}

