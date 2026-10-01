import type { SearchResponse } from './types';

function generateMockResults(query: string, page: number, limit: number = 10): SearchResponse {
  const baseItem = {
    title: `Result for "${query}"`,
    url: "https://example.com/mock-result",
    snippet: `This is a mocked search result for "${query}". It allows testing the UI layout without a running backend.`
  };

  const results = Array.from({ length: limit }, (_, index) => {
    const itemNumber = (page - 1) * limit + index + 1;
    return {
      id: `${page}-${index + 1}`,
      title: `${baseItem.title} - Item #${itemNumber}`,
      url: `${baseItem.url}?item=${itemNumber}`,
      snippet: baseItem.snippet
    };
  });

  return {
    query,
    page,
    limit,
    totalPages: 10,
    totalResults: 100,
    results
  };
}

export async function fetchSearchResults(query: string, page: number = 1): Promise<SearchResponse> {
  // Simulate minor delay, like a backend request
  await new Promise(resolve => setTimeout(resolve, 300));

  return generateMockResults(query, page);
}
