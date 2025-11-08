"use client";

import React from "react";
import {
  Container,
  Grid,
  Typography,
  Box,
  CircularProgress,
  Alert,
} from "@mui/material";
import { Metrics } from "@/app/types/metrics";
import MetricsOverview from "./MetricsOverview";
import LatencyChart from "./LatencyChart";
import ThroughputChart from "./ThroughputChart";
import MemoryUsageChart from "./MemoryUsageChart";
import OperationBreakdown from "./OperationBreakdown";
import PerformanceComparison from "./PerformanceComparison";
import { RB_TREE_COLOR, SKIP_LIST_COLOR } from "@/app/theme";

interface DashboardProps {
  rbTree?: Metrics;
  skipList?: Metrics;
  loading: boolean;
  error?: string;
}

export default function Dashboard({
  rbTree,
  skipList,
  loading,
  error,
}: DashboardProps) {
  if (loading) {
    return (
      <Container maxWidth="xl" sx={{ py: 4 }}>
        <Box
          display="flex"
          justifyContent="center"
          alignItems="center"
          minHeight="50vh"
        >
          <CircularProgress />
        </Box>
      </Container>
    );
  }

  if (error) {
    return (
      <Container maxWidth="xl" sx={{ py: 4 }}>
        <Alert severity="error">{error}</Alert>
      </Container>
    );
  }

  if (!rbTree && !skipList) {
    return (
      <Container maxWidth="xl" sx={{ py: 4 }}>
        <Alert severity="warning">No metrics data available</Alert>
      </Container>
    );
  }

  return (
    <Container maxWidth="xl" sx={{ py: 4 }}>
      <Typography variant="h4" component="h1" gutterBottom align="center" sx={{ mb: 4 }}>
        Order Book Simulator Dashboard
      </Typography>

      {/* Performance Comparison Table - Full Width */}
      <Box sx={{ mb: 4 }}>
        <PerformanceComparison rbTree={rbTree} skipList={skipList} />
      </Box>

      {/* Throughput and Memory Charts - Full Width */}
      <Grid container spacing={3} sx={{ mb: 4 }}>
        <Grid item xs={12} md={6}>
          <ThroughputChart rbTree={rbTree} skipList={skipList} />
        </Grid>
        <Grid item xs={12} md={6}>
          <MemoryUsageChart rbTree={rbTree} skipList={skipList} />
        </Grid>
      </Grid>

      {/* Side-by-side comparison layout */}
      <Grid container spacing={3}>
        {/* Red-Black Tree Column */}
        <Grid item xs={12} md={6}>
          {rbTree && (
            <Box>
              <MetricsOverview metrics={rbTree} color={RB_TREE_COLOR} />
              <Box sx={{ mt: 3 }}>
                <LatencyChart metrics={rbTree} color={RB_TREE_COLOR} />
              </Box>
              <Box sx={{ mt: 3 }}>
                <OperationBreakdown metrics={rbTree} color={RB_TREE_COLOR} />
              </Box>
            </Box>
          )}
        </Grid>

        {/* Skip List Column */}
        <Grid item xs={12} md={6}>
          {skipList && (
            <Box>
              <MetricsOverview metrics={skipList} color={SKIP_LIST_COLOR} />
              <Box sx={{ mt: 3 }}>
                <LatencyChart metrics={skipList} color={SKIP_LIST_COLOR} />
              </Box>
              <Box sx={{ mt: 3 }}>
                <OperationBreakdown
                  metrics={skipList}
                  color={SKIP_LIST_COLOR}
                />
              </Box>
            </Box>
          )}
        </Grid>
      </Grid>
    </Container>
  );
}

