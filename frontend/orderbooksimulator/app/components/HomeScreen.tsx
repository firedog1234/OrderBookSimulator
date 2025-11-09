"use client";

import React, { useState } from "react";
import {
  Container,
  Box,
  Typography,
  TextField,
  Button,
  Paper,
  Alert,
} from "@mui/material";

interface HomeScreenProps {
  onSubmit: (amount: number) => void;
  loading?: boolean;
  error?: string;
}

export default function HomeScreen({
  onSubmit,
  loading = false,
  error,
}: HomeScreenProps) {
  const [amount, setAmount] = useState<string>("");
  const [localError, setLocalError] = useState<string>("");

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    setLocalError("");

    const numAmount = parseInt(amount, 10);

    if (!amount || isNaN(numAmount) || numAmount <= 0) {
      setLocalError("Please enter a valid positive number");
      return;
    }

    onSubmit(numAmount);
  };

  return (
    <Container maxWidth="md" sx={{ py: 8 }}>
      <Paper elevation={3} sx={{ p: 4 }}>
        <Typography
          variant="h3"
          component="h1"
          gutterBottom
          align="center"
          sx={{ mb: 3, fontWeight: 600 }}
        >
          Order Book Simulator
        </Typography>

        <Typography
          variant="body1"
          paragraph
          sx={{ mb: 4, lineHeight: 1.8, fontSize: "1.1rem" }}
        >
          This application compares the performance of two data structures for
          order book management: <strong>Red-Black Trees</strong> and{" "}
          <strong>Skip Lists</strong>. Enter the number of orders you'd like to
          process, and the system will run a benchmark comparing both data
          structures across various metrics including latency, throughput,
          memory usage, and operation breakdowns.
        </Typography>

        <Typography
          variant="body2"
          color="text.secondary"
          paragraph
          sx={{ mb: 4 }}
        >
          The simulator will process the specified number of orders (add,
          cancel, and modify operations) and provide detailed performance
          metrics to help you understand the trade-offs between these two
          implementations.
        </Typography>

        <Box component="form" onSubmit={handleSubmit} sx={{ mt: 4 }}>
          <TextField
            fullWidth
            label="Number of Orders"
            type="number"
            value={amount}
            onChange={(e) => {
              setAmount(e.target.value);
              setLocalError("");
            }}
            error={!!localError}
            helperText={
              localError ||
              "Enter the number of orders to process. The maximum number of orders is 100,000."
            }
            disabled={loading}
            inputProps={{
              min: 1,
              step: 1,
            }}
            sx={{ mb: 3 }}
          />

          {(error || localError) && (
            <Alert severity="error" sx={{ mb: 2 }}>
              {error || localError}
            </Alert>
          )}

          <Button
            type="submit"
            variant="contained"
            size="large"
            fullWidth
            disabled={loading || !amount}
            sx={{ py: 1.5, fontSize: "1.1rem" }}
          >
            {loading ? "Processing..." : "Run Comparison"}
          </Button>
        </Box>
      </Paper>
    </Container>
  );
}
