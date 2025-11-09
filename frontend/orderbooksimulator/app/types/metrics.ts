export type DataStructureName = "Red-Black Tree" | "Skip List";

export interface Metrics {
  name: DataStructureName;

  avgInsertTime: number;
  avgDeleteTime: number;
  avgLookupTime: number;
  minLatency: number;
  maxLatency: number;
  medianLatency: number;
  percentile95: number;
  percentile99: number;

  ordersPerSecond: number;
  totalOrders: number;

  memoryUsageBytes: number;

  addCount: number;
  cancelCount: number;
  modifyCount: number;

  latencies: number[];
}

export interface MetricsResponse {
  rbTree?: Metrics;
  skipList?: Metrics;
}

