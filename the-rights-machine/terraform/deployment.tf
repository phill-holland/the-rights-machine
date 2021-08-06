provider "kubernetes" {
  config_path = "/home/phill/.kube/config"
}

resource "kubernetes_deployment" "the-rights-machine-deployment" {

  metadata {
    name = "the-rights-machine"
    labels = {
      App = "the-rights-machine"
    }
  }

  spec {
    replicas = 3

    selector {
      match_labels = {
        App = "the-rights-machine"
      }
    }

    template {
      metadata {
        labels = {
          App = "the-rights-machine"
        }
      }

      spec {
        container {
          image = "moosedude/the-rights-machine:latest"
          name  = "the-rights-machine"
        }
      }
    }
  }
}

resource "kubernetes_service" "the-rights-machine-service" {
  metadata {
    name = "the-rights-machine"
    labels = {
            App = "the-rights-machine"
        }
  }
  spec {
    selector = {
      App = "the-rights-machine"
    }
    port {
      port        = 5454
      target_port = 5454
    }

    type = "LoadBalancer"
  }
}