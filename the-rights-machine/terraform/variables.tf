variable "region" {
  default = "europe-west2"
}

variable "region_zone" {
  default = "europe-west2-c"
}

variable "project_name" {
  description = "The ID of the Google Cloud project"
  default = "annular-net-321608"
}

variable "credentials_file_path" {
  description = "Path to the JSON file used to describe your account credentials"
  default     = "/home/phill/.gcloud/annular-net-321608-20e4b74265ec.json"
}

variable "env_name" {
  description = "The environment for the GKE cluster"
  default     = "dev"
}

variable "cluster_name" {
  description = "The name for the GKE cluster"
  default     = "the-rights-machine-k8s-cluster"
}

variable "network" {
  description = "The VPC network created to host the cluster in"
  default     = "gke-network"
}

variable "subnetwork" {
  description = "The subnetwork created to host the cluster in"
  default     = "gke-subnet"
}

variable "ip_range_pods_name" {
  description = "The secondary ip range to use for pods"
  default     = "ip-range-pods"
}

variable "ip_range_services_name" {
  description = "The secondary ip range to use for services"
  default     = "ip-range-services"
}