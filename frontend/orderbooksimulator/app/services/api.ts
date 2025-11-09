import { Metrics, MetricsResponse } from "@/app/types/metrics";

const API_BASE_URL = "/api/metrics";
export async function submitOrderAmount(
  amount: number
): Promise<MetricsResponse> {
  try {
    const response = await fetch(API_BASE_URL, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ amount }),
    });

    if (!response.ok) {
      const errorData = await response.json().catch(() => ({}));
      throw new Error(
        errorData.error || `HTTP error! status: ${response.status}`
      );
    }

    const data = await response.json();

    if (Array.isArray(data)) {
      const rbTree = data.find((m: Metrics) => m.name === "Red-Black Tree");
      const skipList = data.find((m: Metrics) => m.name === "Skip List");

      return {
        rbTree: rbTree || undefined,
        skipList: skipList || undefined,
      };
    }

    if (data.rbTree && data.skipList) {
      return data as MetricsResponse;
    }

    const singleMetric = data as Metrics;
    if (singleMetric.name === "Red-Black Tree") {
      return { rbTree: singleMetric };
    } else if (singleMetric.name === "Skip List") {
      return { skipList: singleMetric };
    }

    throw new Error("Invalid response format from API");
  } catch (error) {
    console.error("Error submitting order amount:", error);
    throw error;
  }
}
