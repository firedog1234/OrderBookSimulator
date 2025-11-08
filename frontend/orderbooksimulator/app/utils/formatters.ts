/**
 * Format latency value in microseconds with 2 decimal places
 */
export function formatLatency(µs: number): string {
  return `${µs.toFixed(2)} µs`;
}

/**
 * Format memory bytes to human-readable format (KB, MB, GB)
 */
export function formatMemory(bytes: number): string {
  if (bytes === 0) return "0 B";
  
  const k = 1024;
  const sizes = ["B", "KB", "MB", "GB"];
  
  // Ensure we don't go beyond GB
  const i = Math.min(
    Math.floor(Math.log(bytes) / Math.log(k)),
    sizes.length - 1
  );
  
  const value = bytes / Math.pow(k, i);
  // Show fewer decimal places for larger units
  const decimals = i === 0 ? 0 : i === 1 ? 1 : 2;
  
  return `${value.toFixed(decimals)} ${sizes[i]}`;
}

/**
 * Format large numbers with commas
 */
export function formatNumber(num: number): string {
  return num.toLocaleString("en-US");
}

/**
 * Format throughput (orders per second) with commas
 */
export function formatThroughput(ordersPerSecond: number): string {
  return `${formatNumber(Math.round(ordersPerSecond))} orders/sec`;
}

